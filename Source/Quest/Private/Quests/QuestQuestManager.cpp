// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestQuestManager.h"
#include "UObject/SoftObjectPtr.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "QuestQuestBase.h"

// Sets default values
AQuestQuestManager::AQuestQuestManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

bool AQuestQuestManager::AddNewQuest(TSubclassOf<AQuestQuestBase> QuestClass, bool ShouldStartImmediately)
{
	/** Verify we haven't already received this quest */
	if (!UKismetSystemLibrary::IsValidClass(QuestClass) || AllQuestClasses.Contains(QuestClass))
	{
		return false;
	}

	/** Spawn the Quest actor and update our quest arrays */
	AQuestQuestBase* NewQuest;
	NewQuest = GetWorld()->SpawnActor<AQuestQuestBase>(QuestClass, FVector::ZeroVector, FRotator::ZeroRotator);
	if (!IsValid(NewQuest))
	{
		return false;
	}
	AllQuestClasses.Add(QuestClass);
	CurrentQuests.Add(NewQuest);

	NewQuest->SetUpStartingGoals();

	/** Call the blueprint event that updates the UI */
	BP_OnAddNewQuest(NewQuest);

	/** See if we should select this quest as the current quest */
	if (ShouldStartImmediately || CurrentQuests.Num() <= 1)
	{
		SelectNewCurrentQuest(NewQuest);
	}

	return true;
}

void AQuestQuestManager::
SelectNewCurrentQuest(AQuestQuestBase* Quest)
{
	if (IsValid(Quest))
	{
		CurrentQuest = Quest;
	}
}

