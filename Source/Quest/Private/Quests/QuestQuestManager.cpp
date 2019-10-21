// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestQuestManager.h"
#include "UObject/SoftObjectPtr.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GoalData.h"
#include "GoalStatus.h"
#include "Kismet/KismetSystemLibrary.h"
#include "QuestCharacter.h"
#include "QuestCharacterBase.h"
#include "QuestEnemyCharacter.h"
#include "QuestItem.h"
#include "QuestPlayerController.h"
#include "GoalsToCompleteLater.h"
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
		if (CurrentQuests.IsValidIndex(0) && IsValid(CurrentQuests[0]))
		{
			SelectNewCurrentQuest(Quest, CurrentQuests[0]);
		}
	}
}

void AQuestQuestManager::OnItemObtained(UQuestItem* ObtainedItem)
{
	/** Array that holds goals we need to complete later
* because completing them during the for loop will end the quest and eliminate the array */
	TArray<FGoalsToCompleteLater> GoalsToBeCompleted;

	for (AQuestQuestBase* Quest : CurrentQuests)
	{
		for (int i = 0; i < Quest->CurrentGoals.Num(); i++)
		{
			FGoalData GoalData = Quest->CurrentGoals[i];
			/** Did we find the right kind of item for this goal? */
			if (GoalData.Type == EGoalType::OBTAIN && GoalData.ItemToFind == ObtainedItem)
			{
				/** If the goal completes the quest, wait to complete until we are finished looping */
				if (GoalData.DoesCompletingGoalMeanCompletingQuest)
				{
					FGoalsToCompleteLater QuestToComplete;
					QuestToComplete.Quest = Quest;
					QuestToComplete.Index = Quest->CurrentGoalIndices[i];

					GoalsToBeCompleted.Add(QuestToComplete);
				}
				else
				{
					Quest->CompleteGoal(Quest->CurrentGoalIndices[i], false);
				}
			}
		}
	}
	CompleteGoals(GoalsToBeCompleted);
}

void AQuestQuestManager::OnEnemyKilled(AQuestEnemyCharacter* Enemy)
{
	/** Array that holds goals we need to complete later
* because completing them during the for loop will end the quest and eliminate the array */
	TArray<FGoalsToCompleteLater> GoalsToBeCompleted;

	for (AQuestQuestBase* Quest : CurrentQuests)
	{
		for (int i = 0; i < Quest->CurrentGoals.Num(); i++)
		{
			FGoalData GoalData = Quest->CurrentGoals[i];
			/** Did we kill the right kind of enemy for this goal? */
			if (GoalData.Type == EGoalType::KILL && GoalData.TargetCharacterClass == Enemy->GetClass())
			{
				/** Check whether we have killed enough of this enemy type to complete the goal */
				if (Quest->AmountsOfTargetsObtained[i] + 1 >= GoalData.AmountRequired)
				{
					/** If the goal completes the quest, wait to complete until we are finished looping */
					if (GoalData.DoesCompletingGoalMeanCompletingQuest)
					{
					FGoalsToCompleteLater QuestToComplete;
					QuestToComplete.Quest = Quest;
					QuestToComplete.Index = Quest->CurrentGoalIndices[i];

					GoalsToBeCompleted.Add(QuestToComplete);
					}
					else
					{
						Quest->CompleteGoal(Quest->CurrentGoalIndices[i], false);
					}
				}
				/** If we did not kill enough, update the amount killed */
				else
				{
					Quest->AmountsOfTargetsObtained[i] += 1;
				}
			}
		}
	}
	CompleteGoals(GoalsToBeCompleted);
}

void AQuestQuestManager::OnTalkToCharacter(AQuestCharacterBase* Character)
{
	/** Array that holds goals we need to complete later
* because completing them during the for loop will end the quest and eliminate the array */
	TArray<FGoalsToCompleteLater> GoalsToBeCompleted;

	for (AQuestQuestBase* Quest : CurrentQuests)
	{
		for (int i = 0; i < Quest->CurrentGoals.Num(); i++)
		{
			FGoalData GoalData = Quest->CurrentGoals[i];
			/** Did we find the right kind of item for this goal? */
			if (GoalData.Type == EGoalType::TALK && GoalData.TargetCharacterClass == Character->GetClass())
			{
				/** If the goal completes the quest, wait to complete until we are finished looping */
				if (GoalData.DoesCompletingGoalMeanCompletingQuest)
				{
					FGoalsToCompleteLater QuestToComplete;
					QuestToComplete.Quest = Quest;
					QuestToComplete.Index = Quest->CurrentGoalIndices[i];

					GoalsToBeCompleted.Add(QuestToComplete);
				}
				else
				{
					Quest->CompleteGoal(Quest->CurrentGoalIndices[i], false);
				}
			}
		}
	}
	CompleteGoals(GoalsToBeCompleted);
}

void AQuestQuestManager::CompleteGoals(TArray<FGoalsToCompleteLater> GoalsToComplete)
{
	for (auto& GoalToComplete : GoalsToComplete)
	{
		GoalToComplete.Quest->CompleteGoal(GoalToComplete.Index, false);
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

