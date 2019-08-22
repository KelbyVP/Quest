// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "QuestGameMode.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCombatEnd);

class AQuestCharacterBase;



UCLASS()
class QUEST_API AQuestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AQuestGameMode();




	/** 
	*	This array tracks all enemy characters that are in combat with the party
	*	When all enemies in this array are dead, the Game Mode sends a delegate stating that combat is over
	*/
	UPROPERTY()
	FOnCombatEnd OnCombatEnd;

	UFUNCTION(BlueprintCallable, Category = "QuestGameMode")
		void AddEnemyInCombat(AQuestCharacterBase* EnemyToAdd);

	UFUNCTION(BlueprintCallable, Category = "QuestGameMode")
		void RemoveEnemyInCombat(AQuestCharacterBase* EnemyToRemove);

private:

	TArray<AQuestCharacterBase*> EnemiesInCombat;

};



