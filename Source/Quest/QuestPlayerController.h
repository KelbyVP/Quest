// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AITypes.h"
#include "GameFramework/PlayerController.h"
#include "Navigation/PathFollowingComponent.h"
#include "QuestItem.h"
#include "QuestRegion.h"
#include "UObject/UObjectGlobals.h"
#include "QuestPlayerController.generated.h"

class AQuestCharacter;
class AQuestCharacterBase;
class AQuestMerchantCharacter;
class AQuestQuestManager;
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

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	
	/** Navigate player to the given world location. */
	void SetPlayerDirectedOrder(FHitResult &Hit);

	/** Input handlers for SetDestination action. */
	void OnSetTargetPressed();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Quest Player Controller")
		FVector DestinationLocation;

public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "QuestPlayerController", meta = (DisplayName = "OnTargetSelected"))
		void BP_OnTargetSelected();

	void AddItemsToInventory(TArray<UQuestItem*> ItemsToAdd);

	UFUNCTION(BlueprintImplementableEvent, Category = "QuestPlayerController", meta = (DisplayName = "AddItemToInventory"))
		void BP_AddItemToInventory(UQuestItem* ItemToAdd);

	/** The invisible Spectator Pawn that controls the camera */
	UPROPERTY(BlueprintReadOnly, Category = "QuestPlayerController")
	AQuestSpectatorPawn* ControlledPawn;

	/** The Party Member that is currently selected */
	UPROPERTY(BlueprintReadWrite, Category = "QuestPlayerController")
		AQuestCharacter* SelectedCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "QuestPlayerController")
		TSoftClassPtr<UQuestMoveOrder> MoveOrder;

	UPROPERTY(EditDefaultsOnly, Category = "QuestPlayerController")
		TSoftClassPtr<UQuestOrder> OpenStorageOrder;

	UPROPERTY(BlueprintReadWrite, Category = "QuestPlayerController")
		AQuestQuestManager* QuestManager;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "QuestPlayerController")
		EQuestRegion CurrentRegion;

	/**	Set whether the controller should move the character
	*	(false if mouse click should cause character to do something other than move, such as cast spell, attack from range, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestPlayerController")
	bool bControllerShouldDirectCharacter;

	/** Set whether the controller is targeting */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestPlayerController")
		bool bIsTargeting;

	/** Tracks the amount of gold the player has */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestPlayerController")
		int Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestPlayerController")
		int MaxGold = 999999999;



private:

	void SetPathFollowingComponent();
	void MovetoTargetLocation();

	/** Component used for moving along a path. */
	UPROPERTY(VisibleAnywhere, Category = "QuestPlayerController")
	UPathFollowingComponent* PathFollowingComponent;




};


