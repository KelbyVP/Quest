// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestQuestManager.h"
#include "UObject/SoftObjectPtr.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GoalStatus.h"
#include "Kismet/KismetSystemLibrary.h"
#include "QuestCharacter.h"
#include "QuestPlayerController.h"
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
	NewQuest->QuestManager = this;
	AllQuestClasses.Add(QuestClass);
	CurrentQuests.Add(NewQuest);

	NewQuest->SetUpStartingGoals();

	/** Call the blueprint event that updates the UI */
	BP_OnAddNewQuest(NewQuest);

	/** See if we should select this quest as the current quest */
	if (ShouldStartImmediately || CurrentQuests.Num() <= 1)
	{
		SelectNewCurrentQuest(CurrentQuest, NewQuest);
	}

	return true;
}

void AQuestQuestManager::EndQuest(AQuestQuestBase* Quest)
{
	
	CurrentQuests.Remove(Quest);
	switch (Quest->Status)
	{
	case EGoalStatus::CURRENT:
		CurrentQuests.Add(Quest);
		break;
	case EGoalStatus::SUCCEEDED:
		CompletedQuests.Add(Quest);
		Quest->GiveCompletionRewards(PlayerController, PlayerCharacter);
		break;
	case EGoalStatus::FAILED:
		FailedQuests.Add(Quest);
		break;
	default:
		break;
	}

	BP_OnAddNewQuest(Quest);

	if (Quest == CurrentQuest)
	{
		CurrentQuest = nullptr;
		if (IsValid(CurrentQuests[0]))
		{
			SelectNewCurrentQuest(Quest, CurrentQuests[0]);
		}
	}
}

void AQuestQuestManager::
SelectNewCurrentQuest(AQuestQuestBase* OldQuest, AQuestQuestBase* NewQuest)
{
	if (IsValid(NewQuest))
	{
		CurrentQuest = NewQuest;
		BP_OnNewQuestSelected(OldQuest, NewQuest);
	}
}

