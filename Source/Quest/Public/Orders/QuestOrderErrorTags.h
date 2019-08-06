#pragma once

#include "CoreMinimal.h"
#include "QuestOrder.h"
#include "GameplayTagContainer.h"
#include "QuestOrderErrorTags.generated.h"

/**
 *	Contains any tags (either missing if required or blocking) that explain why an order cannot be issued
 */

USTRUCT(BlueprintType)
struct QUEST_API FQuestOrderErrorTags
{
	GENERATED_BODY()

	/** Contains required tags that are missing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestOrderErrorTags)
	FGameplayTagContainer MissingTags;

	/** Contains blocking tags that are present */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestOrderErrorTags)
	FGameplayTagContainer BlockingTags;

	/** Contains error tags that explain any other failed requirements */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestOrderErrorTags)
	FGameplayTagContainer ErrorTags;

	/** Tells whether any tags prevent the order from issuing */
	bool IsEmpty() const;

	/**
	 * Gets a string that describes the order
	 */
	FString ToString() const;

};