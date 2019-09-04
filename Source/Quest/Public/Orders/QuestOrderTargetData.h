// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "QuestOrderTargetData.generated.h"

class AActor;

/** Targeting data for an order */

USTRUCT(BlueprintType)
struct QUEST_API FQuestOrderTargetData
{
	GENERATED_BODY()

	FQuestOrderTargetData();
	FQuestOrderTargetData(AActor* InActor, const FVector InLocation, const FGameplayTagContainer& InTargetTags);

	/** Target actor */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "QuestOrderTargetData")
	AActor* Actor;

	/** Target location */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "QuestOrderTargetData")
	FVector Location;

	/** Target tags */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "QuestOrderTargetData")
	FGameplayTagContainer TargetTags;

	/**
	 *	Gets a string that describes the target data
	 */
	FString ToString() const;
};