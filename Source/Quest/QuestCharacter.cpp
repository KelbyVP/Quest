// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "QuestCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Camera/CameraComponent.h"
#include "CollisionQueryParams.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Math/Vector.h"
#include "Kismet/GameplayStatics.h"
#include "QuestAttributeSet.h"
#include "QuestMerchantCharacter.h"
#include "QuestPlayerController.h"
#include "QuestStorage.h"
#include "UObject/ConstructorHelpers.h"

AQuestCharacter::AQuestCharacter()
{


	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1000;
	CameraBoom->RelativeRotation = FRotator(-65.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bIsHostile = false;
	bIsReadyForNextAttack = false;
	AttackCooldownTimer = 1.0f;
}

void AQuestCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
	if (bIsCombatModeActive)
	{
		if (bIsReadyForNextAttack)
		{
			SelectTargetCharacterToAttack();
		}
	}
}

void AQuestCharacter::SelectTargetCharacterToAttack()
{
	/** See if we already have a valid character to attack */
	if (TargetActor)
	{
		if (AQuestCharacterBase * TargetCharacter = Cast<AQuestCharacterBase>(TargetActor))
		{
			if (TargetCharacter->bIsHostile)
			{
				if (TargetCharacter->bIsDead)
				{
					TargetActor = nullptr;
				}
				else
				{
					MoveToTarget(TargetCharacter);
					return;
				}
			}
			else
			{
				return;
			}
		}
		else
		{
			return;
		}
	}

	/** Scan for nearby pawns */
	TArray<FHitResult> OutHits;
	FVector Start = GetActorLocation();
	FVector End = Start + FVector(0, 0, 1);
	float SweepSphereRadius = 500.0f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredComponent(GetCapsuleComponent());

	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

	GetWorld()->SweepMultiByObjectType(
		OutHits,
		Start,
		End,
		FQuat::Identity,
		ObjParams,
		FCollisionShape::MakeSphere(SweepSphereRadius),
		QueryParams
	);

	if (OutHits.Num() > 0)
	{
		/** See which nearby pawns are live enemies */
		TArray<AQuestCharacterBase*> LocalLiveEnemies;
		for (auto& Hit : OutHits)
		{
			AQuestCharacterBase* Pawn = Cast<AQuestCharacterBase>(Hit.GetActor());
			if (Pawn && Pawn->bIsHostile && !Pawn->bIsDead)
			{
				LocalLiveEnemies.AddUnique(Pawn);
			}
		}

		/** See which live enemy is closest */
		if (LocalLiveEnemies.Num() > 0)
		{
			AQuestCharacterBase* ClosestEnemy = LocalLiveEnemies[0];
			float DistanceToClosestEnemy = FVector::Dist(GetActorLocation(), ClosestEnemy->GetActorLocation());
			for (auto& Enemy : LocalLiveEnemies)
			{
				float DistanceToThisEnemy = FVector::Dist(GetActorLocation(), Enemy->GetActorLocation());
				if (DistanceToThisEnemy <= DistanceToClosestEnemy)
				{
					ClosestEnemy = Enemy;
					DistanceToClosestEnemy = DistanceToThisEnemy;
				}
			}
			TargetActor = ClosestEnemy;
		}
		else return;

		/** Attack the closest live enemy*/
		MoveToTarget(TargetActor);
	}
	else
	{
		/** We did not find any pawns in range, so disable auto-attack and return */
		UE_LOG(LogTemp, Warning, TEXT("QuestCharacter:SelectTargetToAttack did not find any pawns in range"))
		SetbIsReadyForNextAttack(false);
		return;
	}
}

void AQuestCharacter::AutoAttack()
{
	if (TargetActor)
	{
		if (AQuestCharacterBase* CharacterToAttack = Cast<AQuestCharacterBase>(TargetActor))
		{
			if (CharacterToAttack->bIsHostile && !CharacterToAttack->bIsDead)
			{
				MoveToTarget(CharacterToAttack);
			}
		}
	}
}

void AQuestCharacter::InteractWithTarget(AActor* InteractionTarget)
{
	if (InteractionTarget && InteractionTarget == TargetActor)
	{
		//  Decide what to do if the target is a character
		if (AQuestCharacterBase * TargetCharacter = Cast<AQuestCharacterBase>(TargetActor))
		{
			//  Check to see whether the character is dead
			if (TargetCharacter->bIsDead)
			{
				if (TargetCharacter->bIsHostile)
				{
					if (bIsCombatModeActive)
					{
						bIsReadyForNextAttack = true;
					}
				}
					// TODO:  Figure out what to do if the character is dead (eg. loot the body)
			}
			else
			{
				/**	If the target character is hostile, melee attack */
				if (TargetCharacter->bIsHostile)
				{
					if (!bIsCombatModeActive)
					{
						GameMode->SetbIsCombatModeActive(true);
					}
					SetbIsReadyForNextAttack(true);
					MeleeAttack();
				}

				/** If the Target Character is a Merchant, begin a buy/sell dialogue with the merchant */
				else if (AQuestMerchantCharacter * Merchant = Cast<AQuestMerchantCharacter>(TargetCharacter))
				{
					AQuestPlayerController* PlayerController = Cast<AQuestPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
					if (PlayerController)
					{
						Merchant->OnInteract(PlayerController);
						TargetActor = nullptr;
					}
				}
				else
				{
					/** TODO:  implement functionality for other types of characters (eg., dialogue for NPCs) */
				}
			}
		}
		/** If the target is a storage actor, open it */
		else if (Cast<AQuestStorage>(TargetActor))
		{
			AQuestStorage* Storage = Cast<AQuestStorage>(TargetActor);
			AQuestPlayerController* PlayerController = Cast<AQuestPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			if (PlayerController)
			{
				Storage->OnInteract(PlayerController);
				TargetActor = nullptr;
			}
		}
	}
}

void AQuestCharacter::MoveToTarget(AActor* MoveTarget)
{
	if (MoveTarget)
	{
		// See whether we are in range, and if we are, interact with the target actor
		if (IsOverlappingActor(MoveTarget))
		{
			bIsTargetWithinInteractionSphere = true;
			InteractWithTarget(MoveTarget);
			return;
		}

		// If we are not in range, move to the target
		else
		{
			bIsTargetWithinInteractionSphere = false;
			bIsReadyForNextAttack = false;
			UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), MoveTarget);
			return;
		}
	}
}

void AQuestCharacter::SetbIsReadyForNextAttack(bool NewbIsReadyForNextAttack)
{
	bIsReadyForNextAttack = NewbIsReadyForNextAttack;
}

bool AQuestCharacter::GetbIsReadyForNextAttack()
{
	return bIsReadyForNextAttack;
}

void AQuestCharacter::OnMeleeEnd()
{
	BP_OnMeleeEnd();
}

void AQuestCharacter::OnInteractionSphereBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (TargetActor && TargetActor == OtherActor)
	{
		if (bIsCombatModeActive)
		{
			bIsReadyForNextAttack = true;
		}
		bIsTargetWithinInteractionSphere = true;
		InteractWithTarget(OtherActor);
	}
}

void AQuestCharacter::OnInteractonSphereEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (TargetActor && TargetActor == OtherActor)
	{
		bIsTargetWithinInteractionSphere = false;
	}
}
