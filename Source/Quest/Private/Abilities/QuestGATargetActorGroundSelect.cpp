// Copyright 2019 Kelby Van Patten


#include "QuestGATargetActorGroundSelect.h"
#include <GameFramework/PlayerController.h>
#include "QuestPlayerController.h"
#include "Components/SceneComponent.h"
#include "Components/DecalComponent.h"
#include "GameplayAbility.h"

AQuestGATargetActorGroundSelect::AQuestGATargetActorGroundSelect()
{
	// Create and set a decal component and a root component
	RootComp = CreateDefaultSubobject<USceneComponent>("RootComp");
	SetRootComponent(RootComp);
	Decal = CreateDefaultSubobject<UDecalComponent>("Decal");
	Decal->SetupAttachment(RootComp);

	// Set the default decal size
	Radius = 200.0f;
	Decal->DecalSize = FVector(Radius);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
}


void AQuestGATargetActorGroundSelect::BeginPlay()
{
	Super::BeginPlay();
}

void AQuestGATargetActorGroundSelect::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	PlayerController->bControllerShouldMoveCharacter = true;
}

void AQuestGATargetActorGroundSelect::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	FVector CursorLocation;
	GetCursorLocation(CursorLocation);
	Decal->SetWorldLocation(CursorLocation);
}

void AQuestGATargetActorGroundSelect::StartTargeting(UGameplayAbility* Ability)
{
	// Override ability and controller variables from parent class
	OwningAbility = Ability;
	MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());

	// Keep the mouse from moving the character while targeting is active
	PlayerController = Cast<AQuestPlayerController>(MasterPC);
	PlayerController->bControllerShouldMoveCharacter = false;
	PlayerController->bIsTargeting = true;
	
	// Reset the decal size
	Decal->DecalSize = FVector(Radius);
}

bool AQuestGATargetActorGroundSelect::GetCursorLocation(FVector& CursorLocation)
{
	FHitResult HitResult;
	bool CursorLocationFound = MasterPC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	if (CursorLocationFound)
	{
		CursorLocation = HitResult.ImpactPoint;
	}
	else
	{
		CursorLocation = FVector();
	}
	return CursorLocationFound;
}

void AQuestGATargetActorGroundSelect::ConfirmTargetingAndContinue()
{
	UE_LOG(LogTemp, Warning, TEXT("QGATargetActorGroundSelect::  ConfirmTargetingAndContinue called!"))
	// Get the cursor location
	FVector CursorLocation;
	GetCursorLocation(CursorLocation);

	// Create an array of actors within the target radius of the cursor location
	TArray<FOverlapResult> Overlaps;
	TArray<TWeakObjectPtr<AActor>> OverlappedActors;
	bool TraceComplex = false;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bTraceComplex = TraceComplex;
	CollisionQueryParams.bReturnPhysicalMaterial = false;
	bool TryOverlap = GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		CursorLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECC_Pawn),
		FCollisionShape::MakeSphere(Radius),
		CollisionQueryParams);
	if (TryOverlap)
	{
		for (const auto& o : Overlaps)
		{
			if (auto pawn = Cast<APawn>(o.GetActor()))
			{
				OverlappedActors.AddUnique(pawn);
			}
		}
	}

	// Put the array and cursor location in a Data Handle and broadcast the Data Handle

	/** Note:  This is cleaned up by the FGameplayAbilityTargetDataHandle (via an internal TSharedPtr), so no need to delete here */
	FGameplayAbilityTargetData_LocationInfo* CenterLocation = new FGameplayAbilityTargetData_LocationInfo();
	if (Decal)
	{
		CenterLocation->TargetLocation.LiteralTransform = Decal->GetComponentTransform();
		CenterLocation->TargetLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
	}
	if (OverlappedActors.Num() > 0)
	{
		FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlappedActors);
		TargetData.Add(CenterLocation);
		TargetDataReadyDelegate.Broadcast(TargetData);
	}
	else
	{
		FHitResult HitResult;
		bool CursorLocationFound = MasterPC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromHitResult(OwningAbility, HitResult);
		TargetDataReadyDelegate.Broadcast(TargetData);
	}

	// Enable character movement on mouse click
	PlayerController->bControllerShouldMoveCharacter = true;
	PlayerController->bIsTargeting = false;
}
