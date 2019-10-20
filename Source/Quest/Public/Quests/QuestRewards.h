// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "QuestItem.h"
#include "QuestRewards.generated.h"

/**
 * The rewards that a player receives upon completing a quest
 */

USTRUCT(BlueprintType)
struct QUEST_API FQuestRewards
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestRewards")
		int32 Experience;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestRewards")
		int32 ReputationPoints;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestRewards")
		int32 Gold;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestRewards")
		TArray<UQuestItem*> Items;
};
