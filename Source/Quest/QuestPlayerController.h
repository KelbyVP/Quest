// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "QuestPlayerController.generated.h"

class AQuestCharacter;
class AQuestCharacterBase;
class AQuestMerchantCharacter;

UCLASS()
class AQuestPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AQuestPlayerController();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(FHitResult &Hit);

	/** Input handlers for SetDestination action. */
	void OnSetTargetPressed();
	void OnSetTargetReleased();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Quest Player Controller")
		FVector DestinationLocation;

public:
	AQuestCharacterBase* PawnClicked;

	/**	Set whether the controller can move the character
	*	(false if character is trying to cast spell, attack from range, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestPlayerController")
	bool bControllerCanMoveCharacter;

	/** Tracks the amount of gold the player has */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestPlayerController")
		int Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestPlayerController")
		int MaxGold = 999999999;

	/** Increases the amount of gold the player has */
	UFUNCTION(BlueprintCallable, Category = "QuestPlayerController")
		void IncreaseGold(int Amount);

	/** Decreases the amount of gold the player has */
	UFUNCTION(BlueprintCallable, Category = "QuestPlayerController")
		void DecreaseGold(int Amount);

	/** A blueprint function that updates widgets that reflect the amount of gold the player has */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta=(DisplayName = "UpdateGold"), Category = "QuestPlayerController")
		void UpdateGold();

private:
	AQuestCharacter* ControlledCharacter;

};


