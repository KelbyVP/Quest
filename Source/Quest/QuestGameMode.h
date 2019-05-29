// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "QuestGameMode.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatModeChange, bool, bIsCombatModeActive);

class AQuestCharacterBase;



UCLASS()
class QUEST_API AQuestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AQuestGameMode();




	/** 
	*	During combat mode, this array tracks all enemy characters that have been seen by the party
	*	All enemies in this array must be killed before the game exits Combat Mode 
	*/
	UPROPERTY()
	TArray<AQuestCharacterBase*> SpottedEnemies;

	FOnCombatModeChange OnCombatModeChange;

	UFUNCTION(BlueprintCallable, Category = "QuestGameMode")
		void SetbIsCombatModeActive(bool NewValue);

private:

	/**
	*	Determines whether in combat mode
	*	Combat mode means that characters perform default combat behavior when not assigned other actions
	*/
	bool bIsCombatModeActive;

};



