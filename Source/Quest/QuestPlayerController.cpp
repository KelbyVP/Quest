// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "QuestPlayerController.h"
#include "AITypes.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "QuestGameplayAbility.h"
#include "Math/Vector.h"
#include "QuestCharacter.h"
#include "QuestGameMode.h"
#include "QuestMerchantCharacter.h"
#include "QuestSpectatorPawn.h"
#include "QuestStorage.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"



AQuestPlayerController::AQuestPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
	: Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bControllerShouldMoveCharacter = true;
	bIsTargeting = false;
	Gold = 0;
}

void AQuestPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ControlledPawn = Cast<AQuestSpectatorPawn>(GetPawn());
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
}


void AQuestPlayerController::MoveToMouseCursor()
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

void AQuestPlayerController::SetNewMoveDestination(FHitResult &Hit)
{
	FVector DestLocation = Hit.ImpactPoint;
	if (Hit.bBlockingHit)
	{
		AActor* ActorClicked;

		if (SelectedCharacter)
		{
			ActorClicked = Hit.GetActor();
			PawnClicked = Cast<AQuestCharacterBase>(ActorClicked);
			StorageClicked = Cast<AQuestStorage>(ActorClicked);

			//  If we clicked on a character, set it as the TargetCharacter, move to it, and then interact
			if (PawnClicked)
			{
				// TODO:  Call appropriate order when clicking on a character
			}
			// If we clicked on a storage actor, set it as the TargetStorage
			else if (StorageClicked)
			{
				// TODO:  call appropriate order when clicking on a storage
			}
			// If we did not click on a character or storage actor, move unless we're too close for the animation to play correctly
			else
			{
				SelectedCharacter->AbilitySystemComponent->CurrentMontageStop(1.0f);
				DestinationLocation = DestLocation;
				//MoveToTargetLocation();
			}
		}
	}
}

void AQuestPlayerController::OnSetTargetPressed()
{
		// set flag to keep updating destination until released
		bMoveToMouseCursor = true;
}

void AQuestPlayerController::OnSetTargetReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void AQuestPlayerController::SetPathFollowingComponent()
{
	if (!PathFollowingComponent)
	{
		/** 
		*	The SimpleMoveToLocation function creates a PathFollowingComponent with the right settings,
		*	so moving to current location is a simple way to create the component 
		*/
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, SelectedCharacter->GetActorLocation());
		PathFollowingComponent = FindComponentByClass<UPathFollowingComponent>();
		
		/** Bind the delegate that tells us when a move is finished */
		//PathFollowingComponent->OnRequestFinished.AddUObject(this, &AQuestPlayerController::OnMoveCompleted);
	}

}

