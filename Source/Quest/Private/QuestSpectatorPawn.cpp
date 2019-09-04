// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestSpectatorPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include "Math/Vector.h"
#include "QuestCharacterBase.h"
#include "UObject/ConstructorHelpers.h"

AQuestSpectatorPawn::AQuestSpectatorPawn()
{
	DefaultCameraBoomLength = 1000;
	DefaultCameraRotation = FRotator(-65.f, 0.f, 0.f);
	DestinationLocation = FVector(0.f, 0.f, 0.f);
	bShouldMovePawnTowardSelectedCharacter = false;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = false; // We want arm to rotate when character does
	CameraBoom->TargetArmLength = DefaultCameraBoomLength;
	CameraBoom->RelativeRotation = DefaultCameraRotation;
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
}

void AQuestSpectatorPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (CursorToWorld != nullptr)
	{
		if (APlayerController * PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}

	// TODO:  Do we really need to do this in the Tick?  maybe just have CenterOnCharacter call MovetoSelectedCharacterLocation
	if (bShouldMovePawnTowardSelectedCharacter)
	{
		MoveToSelectedCharacterLocation(DeltaSeconds);
	}
}

void AQuestSpectatorPawn::CenterOnCharacter(AQuestCharacterBase* SelectedCharacter)
{
	if (SelectedCharacter)
	{
		FVector NewLocation = SelectedCharacter->GetActorLocation();
		DestinationLocation = NewLocation;
		bShouldMovePawnTowardSelectedCharacter = true;
	}
}

void AQuestSpectatorPawn::MoveToSelectedCharacterLocation(float DeltaTime)
{
	FVector NewLocation = FMath::VInterpTo(GetActorLocation(), DestinationLocation, DeltaTime, 20);
	SetActorLocation(NewLocation);
	float DistanceToGo = FVector(GetActorLocation() - DestinationLocation).Size();
	if (DistanceToGo <= 30)
	{
		bShouldMovePawnTowardSelectedCharacter = false;
	}
}
