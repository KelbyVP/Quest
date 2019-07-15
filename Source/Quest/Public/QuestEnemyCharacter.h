// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestCharacterBase.h"
#include "QuestEnemyCharacter.generated.h"



/**
 * 
 */
UCLASS()
class QUEST_API AQuestEnemyCharacter : public AQuestCharacterBase
{
	GENERATED_BODY()
public:

	// Class constructor and overrides
		AQuestEnemyCharacter();

	// Is this enemy a regular character class (as opposed to a monster)
		bool bIsCharacterClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
