// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "QuestPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "QuestCharacter.h"
#include "Engine/World.h"

AQuestPlayerController::AQuestPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bControllerCanMoveCharacter = true;
}

void AQuestPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void AQuestPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetTarget", IE_Pressed, this, &AQuestPlayerController::OnSetTargetPressed);
	InputComponent->BindAction("SetTarget", IE_Released, this, &AQuestPlayerController::OnSetTargetReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AQuestPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AQuestPlayerController::MoveToTouchLocation);
}


void AQuestPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (AQuestCharacter* MyPawn = Cast<AQuestCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void AQuestPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void AQuestPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	if (ControlledCharacter)
	{
		float const Distance = FVector::Dist(DestLocation, ControlledCharacter->GetActorLocation());

		// If we have a target character, see if it is within range
		if (ControlledCharacter->TargetCharacter)
		{
			if (Distance <= ControlledCharacter->InteractionSphereRadius)
			{
				ControlledCharacter->bIsTargetCharacterWithinInteractionSphere = true;
			}
			else
			{
				ControlledCharacter->bIsTargetCharacterWithinInteractionSphere = false;
			}
		}

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
			DestinationLocation = DestLocation;
		}
	}
}

void AQuestPlayerController::OnSetTargetPressed()
{
	if (bControllerCanMoveCharacter)
	{
		// set flag to keep updating destination until released
		bMoveToMouseCursor = true;
	}

	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		// We hit something, so set ActorClicked to the thing we clicked on
		AActor* ActorClicked;
		ActorClicked = Hit.GetActor();

		// check whether the thing we clicked is a character, and if it is, set it as the target
		PawnClicked = Cast<AQuestCharacterBase>(ActorClicked);
		ControlledCharacter = Cast<AQuestCharacter>(GetPawn());

		if (PawnClicked)
		{
			ControlledCharacter->TargetCharacter = PawnClicked;
		}
		else
		{
			ControlledCharacter->TargetCharacter = nullptr;
		}
	}
}

void AQuestPlayerController::OnSetTargetReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}
