// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "QuestGameplayAbility.h"
#include "QuestOrder.h"
#include "QuestOrderData.generated.h"

/**
 * An order that can be given to a character to execute
 */

USTRUCT(BlueprintType)
struct QUEST_API FQuestOrderData
{
	GENERATED_BODY()

	FQuestOrderData();
	FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType);
	FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType, TSubclassOf<UQuestGameplayAbility> InAbility);
	FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType, AActor* InTargetActor);
	FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType, FVector InTargetLocation);
	FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType, AActor* InTargetActor, FVector InTargetLocation);


	/** The order type */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestOrderData")
		TSoftClassPtr<UQuestOrder> OrderType;

	/** Whether the order should use the location (i.e., location is valid) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestOrderData", meta = (InlineEditConditionToggle = true))
	uint8 bUseLocation : 1;

	/** The order's target location; bUseLocation should be used to verify that the location is valid */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestOrderData", meta = (EditCondition = bUseLocation))
	FVector TargetLocation;

	/** The order's target actor; "null" if none. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestOrderData")
	AActor* TargetActor;

	/** The order's ability; "null" if none. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestOrderData")
		TSubclassOf<UQuestGameplayAbility> Ability;

	/** Gets a text description of the order. */
	FString ToString() const;

	/** Operator overloads */
	bool operator==(const FQuestOrderData& Other) const;
	bool operator!=(const FQuestOrderData& Other) const;
};
