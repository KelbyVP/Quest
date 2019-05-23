// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "QuestPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "QuestCharacter.h"
#include "QuestMerchantCharacter.h"
#include "Engine/World.h"

AQuestPlayerController::AQuestPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bControllerCanMoveCharacter = true;
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
	if (!ControlledCharacter)
	{
		ControlledCharacter = Cast<AQuestCharacter>(GetPawn());
	}
	if (ControlledCharacter->TargetCharacter)
	{	
		if (ControlledCharacter->bIsTargetCharacterWithinInteractionSphere)
		{
			if (!ControlledCharacter->TargetCharacter->bIsDead)
			{
				/** If the Target Character is an enemy, then begin melee */
				if (ControlledCharacter->TargetCharacter->bIsHostile)
				{
					ControlledCharacter->MeleeAttack();
				}

				/** If the Target Character is a Merchant, then begin a buy/sell dialog with that merchant */
				else if (Cast<AQuestMerchantCharacter>(ControlledCharacter->TargetCharacter))
				{
					AQuestMerchantCharacter *Merchant = Cast<AQuestMerchantCharacter>(ControlledCharacter->TargetCharacter);
					Merchant->OnInteract(this);					
				}

				// TODO:  initiate dialog, etc. if the target is not dead and is not hostile
			}
			else
			{
				// TODO:  loot the body if the target is dead
			}
		}
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
		ActorClicked = Hit.GetActor();
		PawnClicked = Cast<AQuestCharacterBase>(ActorClicked);
		ControlledCharacter = Cast<AQuestCharacter>(GetPawn());

		if (ControlledCharacter)
		{
			//  If we clicked on a character, set it as the target character
			if (PawnClicked)
			{
				float const Distance = FVector::Dist(PawnClicked->GetActorLocation(), ControlledCharacter->GetActorLocation());
				ControlledCharacter->TargetCharacter = PawnClicked;

				// if we are out of range, move to the target character

				if (Distance > ControlledCharacter->InteractionSphereRadius)
				{
					ControlledCharacter->bIsTargetCharacterWithinInteractionSphere = false;
					UAIBlueprintHelperLibrary::SimpleMoveToActor(this, ControlledCharacter->TargetCharacter);
				}
				else
				{
					ControlledCharacter->bIsTargetCharacterWithinInteractionSphere = true;
				}
			}
			// If we did not click on a character, move unless we're too close for the animation to play correctly
			else
			{
				ControlledCharacter->TargetCharacter = nullptr;
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
	if (bControllerCanMoveCharacter)
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
