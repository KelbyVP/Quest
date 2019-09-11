// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "QuestPlayerController.h"
#include "AITypes.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"
#include "QuestAutoOrderComponent.h"
#include "QuestCharacter.h"
#include "QuestGameMode.h"
#include "QuestGameplayAbility.h"
#include "QuestMerchantCharacter.h"
#include "QuestMoveOrder.h"
#include "QuestOrder.h"
#include "QuestOrderHandlingComponent.h"
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
	//if (bMoveToMouseCursor)
	//{
	//	MoveToMouseCursor();
	//}
}

void AQuestPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetTarget", IE_Pressed, this, &AQuestPlayerController::OnSetTargetPressed);
	/*InputComponent->BindAction("SetTarget", IE_Released, this, &AQuestPlayerController::OnSetTargetReleased);*/
}


//void AQuestPlayerController::MoveToMouseCursor()
//{
//		// Trace to see what is under the mouse cursor
//		 FHitResult Hit;
//		 GetHitResultUnderCursor(ECC_Visibility, false, Hit);
//
//		if (Hit.bBlockingHit)
//		{
//			// We hit something, move there
//			SetNewMoveDestination(Hit);
//		}
//}

void AQuestPlayerController::SetPlayerDirectedOrder(FHitResult &Hit)
{
	if (Hit.bBlockingHit)
	{
		AActor* ActorClicked;

		if (SelectedCharacter)
		{
			ActorClicked = Hit.GetActor();

			//  If we clicked on a character, set appropriate order
			AQuestCharacterBase* CharacterClicked;
			CharacterClicked = Cast<AQuestCharacterBase>(ActorClicked);
			if (CharacterClicked)
			{
				SelectedCharacter->OrderHandlingComponent->IssuePlayerDirectedOrderWithTarget(CharacterClicked);
				return;
			}

			// If we clicked on a storage actor, set it as the TargetStorage
			AQuestStorage* StorageClicked;
			StorageClicked = Cast<AQuestStorage>(ActorClicked);
			if (StorageClicked)
			{
				// TODO:  call appropriate order when clicking on a storage
				return;
			}

			// If we did not click on a character or storage actor, move unless we're too close for the animation to play correctly			
			SelectedCharacter->AbilitySystemComponent->CurrentMontageStop(1.0f);
			FVector DestLocation = Hit.ImpactPoint;
			SelectedCharacter->OrderHandlingComponent->IssuePlayerDirectedOrderWithTarget(DestLocation, MoveOrder);

		}
	}
}

void AQuestPlayerController::OnSetTargetPressed()
{
		// set flag to keep updating destination until released
		//bMoveToMouseCursor = true;
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	if (Hit.bBlockingHit)
	{
		// If we hit something, issue appropriate order
		SetPlayerDirectedOrder(Hit);
	}

}

//void AQuestPlayerController::OnSetTargetReleased()
//{
//	// clear flag to indicate we should stop updating the destination
//	bMoveToMouseCursor = false;
//}

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

