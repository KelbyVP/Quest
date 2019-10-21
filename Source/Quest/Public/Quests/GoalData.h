// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GoalType.h"
#include "GoalData.generated.h"

class AActor;
class UQuestItem;
/**
 * Holds information about a goal
 */

USTRUCT(BlueprintType)
struct QUEST_API FGoalData
{
	GENERATED_BODY()

	/** The type of goal */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestGoalData")
		EGoalType Type;

	/** Name of an item or character to be used in templated, non-custom goal descriptions */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestGoalData")
		FText AdditionalNameForGoalDescription;

	/** A custom description of the goal to be used in the UI */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestGoalData")
		FText CustomGoalDescription;

	/** Tells us whether the goal has a location associated with it */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestGoalData")
		bool HasLocation;

	/** The location associated with the goal */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestGoalData")
		FVector Location;

	/** Tells us the class of character we are supposed to kill or talk to */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestGoalData")
		TSubclassOf<AActor> TargetCharacterClass;

	/** Tells us the type of item we are supposed to find */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestGoalData")
		UQuestItem* ItemToFind;

	/** If the goal type is to kill or to find items, the number necessary to complete the goal */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestGoalData")
		int32 AmountRequired;

	/** Tells us whether to update the quest's description in the UI once the goal is complete 
	* because the story associated with the quest has changed 
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestGoalData")
		bool ShouldUpdateQuestDescriptionUponCompletion;

	/** The additional description for the quest to add once the goal is completed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestGoalData")
		FText UpdatedQuestDescription;

	/** The indices of the goals that should become available once this goal is completed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestGoalData")
		TArray<int32> FollowingGoalIndices;

	/** Tells us whether the quest fails if the player fails to complete the goal */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestGoalData")
		bool DoesFailingGoalCauseQuestToFail;

	/** Tells us whether completing the goal completes the quest */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestGoalData")
		bool DoesCompletingGoalMeanCompletingQuest;
};