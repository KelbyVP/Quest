#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "QuestOrderTagRequirements.generated.h"

/**
 *	Tag requirements that must be met for the order to issue
 */

USTRUCT(BlueprintType)
struct QUEST_API FQuestOrderTagRequirements
{
	GENERATED_BODY()

	/** The character being ordered must have all of these tags before the order can issue */
	UPROPERTY(BlueprintReadWrite, Category = TagRequirements)
	FGameplayTagContainer SourceRequiredTags;

	/** The order will not issue if the character being ordered has any of these tags */
	UPROPERTY(BlueprintReadWrite, Category = TagRequirements)
	FGameplayTagContainer SourceBlockedTags;

	/** The target character must have all of these tags before the order can issue */
	UPROPERTY(BlueprintReadWrite, Category = TagRequirements)
	FGameplayTagContainer TargetRequiredTags;

	/** The target character may not have any of these tags for the order to issue */
	UPROPERTY(BlueprintReadWrite, Category = TagRequirements)
	FGameplayTagContainer TargetBlockedTags;

	/** Empty tag requirements */
	static const FQuestOrderTagRequirements EMPTY_TAG_REQUIREMENTS;

};
