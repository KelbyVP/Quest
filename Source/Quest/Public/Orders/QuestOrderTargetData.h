// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "QuestOrderTargetData.generated.h"

class AActor;

/** Target data for an order */
USTRUCT(BlueprintType)
struct QUEST_API FQuestOrderTargetData
{
	GENERATED_BODY()

	FQuestOrderTargetData();
	FQuestOrderTargetData(AActor* InActor, const FVector InLocation, const FGameplayTagContainer& InTargetTags);

	/** The target actor */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = QuestOrderTargetData)
		AActor* Actor;

	/** The target location */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = QuestOrderTargetData)
		FVector Location;

	/** The target tags */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = QuestOrderTargetData)
		FGameplayTagContainer TargetTags;

	/** Get target data as text */
	FString ToString() const;
};


