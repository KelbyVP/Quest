// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestQuestBase.h"
#include "Containers/Array.h"
#include "QuestCompletedGoal.h"
#include "QuestData.h"
#include "QuestItem.h"

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
	return QuestData.Goals[Index];
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

