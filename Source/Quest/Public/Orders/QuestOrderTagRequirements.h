// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "QuestOrderTagRequirements.generated.h"

/**
 * Contains the tag requirements for an order
 */

USTRUCT(BlueprintType)
struct QUEST_API FQuestOrderTagRequirements
{
	GENERATED_BODY()

	/**  The ordered character must have these tags to execute the order */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "QuestOrderTagRequirements")
		FGameplayTagContainer SourceTagsRequired;

	/**  The ordered character cannot have these tags to execute the order */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "QuestOrderTagRequirements")
		FGameplayTagContainer SourceTagsBlocked;

	/**  The target character must have these tags for the ordered character to execute the order */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "QuestOrderTagRequirements")
		FGameplayTagContainer TargetTagsRequired;

	/**  The target character cannot have these tags for the ordered character to execute the order */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "QuestOrderTagRequirements")
		FGameplayTagContainer TargetTagsBlocked;

	static const FQuestOrderTagRequirements EMPTY_TAG_REQUIREMENTS;
};