// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GoalData.h"
#include "QuestCompletedGoal.generated.h"


/**
 * Holds information about a completed goal
 */

USTRUCT(BlueprintType)
struct QUEST_API FQuestCompletedGoal
{
	GENERATED_BODY()

	FQuestCompletedGoal();

	/** The index of the goal in the quest */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestCompletedGoal")
		int32 GoalIndex;

	/** Information about the goal */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestCompletedGoal")
		FGoalData GoalData;

	/** Tells us whether the goal was completed successfully */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestCompletedGoal")
		bool WasSuccessful;

};
