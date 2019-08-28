// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "QuestOrder.h"
#include "GameplayTagContainer.h"
#include "QuestOrderErrorTags.generated.h"

/**
 * Gathers and identifies error tags that preclude an order from being issued
 */

USTRUCT(BlueprintType)
struct QUEST_API FQuestOrderErrorTags
{
	GENERATED_BODY()

	/**  Necessary tags that are absent */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestOrderErrorTags")
		FGameplayTagContainer MissingTags;

	/**  Prohibited tags that are present */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestOrderErrorTags")
		FGameplayTagContainer BlockingTags;

	/**  Other error tags */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestOrderErrorTags")
		FGameplayTagContainer ErrorTags;

	/** Tells whether there are any error tags */
	bool IsEmpty() const;

	/**
	 *  Provides a text description of the error tags.
	 */
	FString ToString() const;
};
