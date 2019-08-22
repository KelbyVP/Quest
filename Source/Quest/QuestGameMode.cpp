// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "QuestGameMode.h"
#include "QuestPlayerController.h"
#include "QuestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AQuestGameMode::AQuestGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AQuestPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

}

void AQuestGameMode::AddEnemyInCombat(AQuestCharacterBase* EnemyToAdd)
{
	EnemiesInCombat.AddUnique(EnemyToAdd);
}

void AQuestGameMode::RemoveEnemyInCombat(AQuestCharacterBase* EnemyToRemove)
{
	if (EnemiesInCombat.Contains(EnemyToRemove))
	{
		EnemiesInCombat.Remove(EnemyToRemove);
	}
	if (EnemiesInCombat.Num() <= 0)
	{
		OnCombatEnd.Broadcast();
	}
}
