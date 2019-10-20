// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GoalData.h"
#include "QuestCharacterBase.h"
#include "QuestRegion.h"
#include "QuestRewards.h"
#include "QuestTypes.h"
#include "QuestData.generated.h"


/**
 * Holds information about a quest
 */

USTRUCT(BlueprintType)
struct QUEST_API FQuestQuestData
{
	GENERATED_BODY()

	/** The name of the quest */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestQuestData")
		FText Name;

	/** The type of quest */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestQuestData")
		EQuestTypes Type;

	/** A short summary of the quest to be used in the UI */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestQuestData")
		FText Summary;

	/** A description of the quest to be used in the UI */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestQuestData")
		FText Description;

	/** The Region in which the quest is to be performed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestQuestData")
		EQuestRegion Region;

	/** The rewards for completing the quest */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestQuestData")
		FQuestRewards Rewards;

	/** The recommended level that the player character should achieve before beginning the quest  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestQuestData")
		int32 SuggestedLevel;

	/** Whether the quest is assigned by a particular character */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestQuestData")
		bool HasAssigningCharacter;

	/** The character assigning the quest */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestQuestData")
		TSubclassOf<AQuestCharacterBase> AssigningCharacterClass;

	/** Goals associated with this quest */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestQuestData")
	TArray<FGoalData> Goals;
};