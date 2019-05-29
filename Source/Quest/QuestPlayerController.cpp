// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "QuestPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine.h"
#include "Engine/World.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Math/Vector.h"
#include "QuestCharacter.h"
#include "QuestGameMode.h"
#include "QuestMerchantCharacter.h"
#include "QuestStorage.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"


AQuestPlayerController::AQuestPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bControllerShouldMoveCharacter = true;
	ControlledCharacter = Cast<AQuestCharacter>(GetPawn());
	Gold = 0;
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
			SetNewMoveDestination(Hit);
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
		SetNewMoveDestination(HitResult);
	}
}

void AQuestPlayerController::SetNewMoveDestination(FHitResult &Hit)
{
	FVector DestLocation = Hit.ImpactPoint;
	if (Hit.bBlockingHit)
	{
		AActor* ActorClicked;
		if (!ControlledCharacter)
		{
			ControlledCharacter = Cast<AQuestCharacter>(GetPawn());
		}

		if (ControlledCharacter)
		{
			ActorClicked = Hit.GetActor();
			PawnClicked = Cast<AQuestCharacterBase>(ActorClicked);
			StorageClicked = Cast<AQuestStorage>(ActorClicked);

			//  If we clicked on a character, set it as the TargetCharacter, move to it, and then interact
			if (PawnClicked)
			{
				ControlledCharacter->TargetActor = PawnClicked;
				MoveToTarget(PawnClicked);
			}
			// If we clicked on a storage actor, set it as the TargetStorage
			else if (StorageClicked)
			{
				ControlledCharacter->TargetActor = StorageClicked;
				MoveToTarget(StorageClicked);
			}
			// If we did not click on a character or storage actor, move unless we're too close for the animation to play correctly
			else
			{
				ControlledCharacter->TargetActor = nullptr;
				ControlledCharacter->SetbIsReadyForNextAttack(false);
				float const Distance = FVector::Dist(DestLocation, ControlledCharacter->GetActorLocation());
				if ((Distance > 120.0f))
				{
					UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
					DestinationLocation = DestLocation;
				}
			}
		}
	}
}

void AQuestPlayerController::OnSetTargetPressed()
{
	if (bControllerShouldMoveCharacter)
	{
		// set flag to keep updating destination until released
		bMoveToMouseCursor = true;
	}
}

void AQuestPlayerController::OnSetTargetReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void AQuestPlayerController::IncreaseGold(int Amount)
{
	Gold += Amount;
	FMath::Clamp(Gold, 0, MaxGold);
	UpdateGold();
}

void AQuestPlayerController::DecreaseGold(int Amount)
{
	Gold -= Amount;
	FMath::Clamp(Gold, 0, MaxGold);
	UpdateGold();
}

void AQuestPlayerController::MoveToTarget(AActor *MoveTarget)
{	
	if (MoveTarget)
	{
		ControlledCharacter->MoveToTarget(MoveTarget);
	}
	return;
}

