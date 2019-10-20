// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestQuestBase.h"
#include "Containers/Array.h"
#include "GameFramework/Actor.h"
#include "QuestCharacter.h"
#include "QuestCompletedGoal.h"
#include "QuestData.h"
#include "QuestGameInstanceBase.h"
#include "QuestItem.h"
#include "QuestPlayerController.h"
#include "QuestQuestManager.h"

// Sets default values
AQuestQuestBase::AQuestQuestBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AQuestQuestBase::SetUpStartingGoals()
{
	CurrentDescription = QuestData.Description;
	
	CurrentGoalIndices.Empty();
	for (int32& Index : StartingGoalIndices)
	{
		AddGoal(Index);
	}

	UpdateGoals();
	AmountsOfTargetsObtained.SetNum(CurrentGoalIndices.Num());
}

void AQuestQuestBase::UpdateGoals()
{
	CurrentGoals.Empty();
	for (int& Index : CurrentGoalIndices)
	{
		CurrentGoals.Add(QuestData.Goals[Index]);
	}
}

void AQuestQuestBase::AddGoal(int32 Index)
{
	/** If the goal is to obtain an item we already have, mark the goal completed */
	if (IsItemAlreadyObtained(Index))
	{
		FQuestCompletedGoal CompletedGoal = FQuestCompletedGoal();
		CompletedGoal.GoalIndex = Index;
		CompletedGoal.GoalData = GetGoalAtIndex(Index);
		CompletedGoal.WasSuccessful = true;
		CompletedGoals.Add(CompletedGoal);
		BP_OnGoalCompleted(Index);
		for (int32& FollowingIndex : CompletedGoal.GoalData.FollowingGoalIndices)
		{
			AddGoal(FollowingIndex);
		}
	}

	else
	{
		/** If we have more current goals than sets of things we are targeting, add an element to the array of what we are targeting */
		CurrentGoalIndices.Add(Index);
		if (CurrentGoalIndices.Num() > AmountsOfTargetsObtained.Num())
		{
			AmountsOfTargetsObtained.Add(0);
		}

		CurrentGoals.Add(GetGoalAtIndex(Index));
	}
}

FGoalData AQuestQuestBase::GetGoalAtIndex(int32 Index)
{
	if (QuestData.Goals.IsValidIndex(Index))
	{
		return QuestData.Goals[Index];
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestQuestBase::GetGoalAtIndex: No valid goal at index, so returning empty goal data!"))
		FGoalData EmptyGoalData;
		return EmptyGoalData;
	}
}

bool AQuestQuestBase::IsItemAlreadyObtained(int32 GoalIndex)
{
	FGoalData GoalData = GetGoalAtIndex(GoalIndex);
	if (GoalData.Type != EGoalType::OBTAIN)
	{
		return false;
	}

	return BP_HasInventoryItem(GoalData.ItemToFind);	
}

bool AQuestQuestBase::CompleteGoal(int32 GoalIndex, bool IsGoalFailed)
{
	if (!CurrentGoalIndices.Contains(GoalIndex))
	{
		return false;
	}
	FQuestCompletedGoal CompletedGoal = FQuestCompletedGoal();
	CompletedGoal.GoalData = QuestData.Goals[GoalIndex];
	CompletedGoal.GoalIndex = GoalIndex;
	CompletedGoal.WasSuccessful = !IsGoalFailed;
	CompletedGoals.Add(CompletedGoal);

	if (QuestData.Goals[GoalIndex].ShouldUpdateQuestDescriptionUponCompletion && !IsGoalFailed)
	{
		BP_UpdateCurrentDescription(GoalIndex);
	}

	
	//  Here, we are trying to do the following, which C++ won't let us do:  CurrentGoals.Remove(CompletedGoal.GoalData);
	int32 Index = CurrentGoalIndices.Find(GoalIndex);
	if (CurrentGoals.Num() > 0 && CurrentGoals.IsValidIndex(Index))
	{
		CurrentGoals.RemoveAt(Index);
	}

	if (AmountsOfTargetsObtained.Num() > 0 && AmountsOfTargetsObtained.IsValidIndex(Index))
	{
		AmountsOfTargetsObtained.RemoveAt(Index);
	}

	CurrentGoalIndices.Remove(GoalIndex);

	if (IsGoalFailed)
	{
		BP_OnGoalFailed(GoalIndex);
		if (CompletedGoal.GoalData.DoesFailingGoalCauseQuestToFail)
		{
			OnGoalEndsQuest(IsGoalFailed);
			return true;
		}
	}
	else
	{
		BP_OnGoalSucceeded(GoalIndex);
		if (QuestData.Goals[GoalIndex].DoesCompletingGoalMeanCompletingQuest)
		{
			OnGoalEndsQuest(IsGoalFailed);
			return true;
		}
		else
		{
			/** Add following goals */
			for (int32& FollowingIndex : QuestData.Goals[GoalIndex].FollowingGoalIndices)
			{
				AddGoal(FollowingIndex);
			}
		}
	}
	BP_UpdateGoalsInJournal();
	// add the rest of the functions from the demo's Complete Sub Goal in BP_MasterQuest after journal subgoals generated
	return true;
}

void AQuestQuestBase::OnGoalEndsQuest(bool IsGoalFailed)
{
	/** Fail all remaining goals */
	for (int& CurrentGoalIndex : CurrentGoalIndices)
	{
		FQuestCompletedGoal GoalToFail;
		GoalToFail.GoalData = GetGoalAtIndex((CurrentGoalIndex));
		GoalToFail.GoalIndex = CurrentGoalIndex;
		GoalToFail.WasSuccessful = false;
		CompletedGoals.Add(GoalToFail);
	}
	CurrentGoalIndices.Empty();
	if (AmountsOfTargetsObtained.Num() > 0)
	{
		AmountsOfTargetsObtained.Empty();
	}
	CurrentGoals.Empty();

	if (IsGoalFailed) { Status = EGoalStatus::FAILED; }
	else { Status = EGoalStatus::SUCCEEDED; }

	BP_RemoveWidgets();
	QuestManager->EndQuest(this);
}

void AQuestQuestBase::GiveCompletionRewards(AQuestPlayerController* PlayerController, AQuestCharacter* PlayerCharacter)
{
	if (QuestData.Rewards.Experience > 0)
	{
		if (IsValid(PlayerCharacter))
		{
			PlayerCharacter->Experience += QuestData.Rewards.Experience;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("QuestQuestBase::GiveCompletionRewards:  No PlayerCharacter found!"))
		}
	}

	if (QuestData.Rewards.ReputationPoints > 0)
	{
		if (IsValid(PlayerCharacter))
		{
			FRegionReputation ReputationToAdd;
			ReputationToAdd.Region = QuestData.Region;
			ReputationToAdd.ReputationPoints = QuestData.Rewards.ReputationPoints;
			PlayerCharacter->AddReputationPoints(ReputationToAdd);
		}
		else
		{UE_LOG(LogTemp, Warning, TEXT("QuestQuestBase::GiveCompletionRewards:  No PlayerCharacter found!")) }
	}

	if (QuestData.Rewards.Gold > 0)
	{
		UQuestGameInstanceBase* GameInstance = Cast<UQuestGameInstanceBase>(GetGameInstance());
		if (IsValid(GameInstance))
		{
			GameInstance->IncreaseGold(QuestData.Rewards.Gold);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("QuestQuestBase::GiveCompletionRewards:  Game instance not found!"))
		}
	}

	if (QuestData.Rewards.Items.Num() > 0)
	{
		PlayerController->AddItemsToInventory(QuestData.Rewards.Items);
	}
}

