// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AITypes.h"
#include "GameFramework/PlayerController.h"
#include "Navigation/PathFollowingComponent.h"
#include "UObject/UObjectGlobals.h"
#include "QuestPlayerController.generated.h"

class AQuestCharacter;
class AQuestCharacterBase;
class AQuestMerchantCharacter;
class UQuestOrder;
class AQuestSpectatorPawn;
class AQuestStorage;
class UQuestMoveOrder;

UCLASS()
class AQuestPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AQuestPlayerController(const FObjectInitializer& ObjectInitialize);
	virtual void BeginPlay() override;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	//uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	///** Navigate player to the current mouse cursor location. */
	//void MoveToMouseCursor();
	
	/** Navigate player to the given world location. */
	void SetPlayerDirectedOrder(FHitResult &Hit);

	/** Input handlers for SetDestination action. */
	void OnSetTargetPressed();
	//void OnSetTargetReleased();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Quest Player Controller")
		FVector DestinationLocation;

public:

	/** The invisible Spectator Pawn that controls the camera */
	UPROPERTY(BlueprintReadOnly, Category = "QuestPlayerController")
	AQuestSpectatorPawn* ControlledPawn;

	/** The Party Member that is currently selected */
	UPROPERTY(BlueprintReadWrite, Category = "QuestPlayerController")
		AQuestCharacter* SelectedCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "QuestPlayerController")
		TSoftClassPtr<UQuestMoveOrder> MoveOrder;


	/**	Set whether the controller should move the character
	*	(false if mouse click should cause character to do something other than move, such as cast spell, attack from range, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestPlayerController")
	bool bControllerShouldMoveCharacter;

	/** Set whether the controller is targeting */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestPlayerController")
		bool bIsTargeting;

	/** Tracks the amount of gold the player has */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestPlayerController")
		int Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestPlayerController")
		int MaxGold = 999999999;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "QuestPlayerController", meta = (DisplayName = "OnTargetSelected"))
		void BP_OnTargetSelected();



private:

	void SetPathFollowingComponent();
	void MovetoTargetLocation();

	/** Component used for moving along a path. */
	UPROPERTY(VisibleAnywhere, Category = "QuestPlayerController")
	UPathFollowingComponent* PathFollowingComponent;




};


