// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "QuestOrder.h"
#include "QuestOrderData.generated.h"

class AActor;

/**
 *	An order that can be issued to characters
 */

USTRUCT(BlueprintType)
struct QUEST_API FQuestOrderData
{
	GENERATED_BODY()

	/** Class Constructors */
	FQuestOrderData();
	FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType);
	FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType, AActor* InTarget);
	FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType, FVector InLocation);
	FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType, AActor* InTarget, FVector InLocation);
	FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType, int32 InIndex);
	FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType, int32 InIndex, AActor* InTarget);
	FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType, int32 InIndex, FVector InLocation);
	FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType, int32 InIndex, AActor* InTarget, FVector InLocation);

	/** Order type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestOrderData)
		TSoftClassPtr<UQuestOrder> OrderType;

	/** Confirms location is valid */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestOrderData, meta = (InlineEditConditionToggle = true))
		uint8 bUseLocation : 1;

	/** Target location for order; use bUseLocation to check if it is really set and valid */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestOrderData, meta = (EditCondition = bUseLocation))
		FVector Location;

	/** Target actor for order; null if no order specified */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestOrderData)
		AActor* Target;

	/** Additional index used to differentiate orders; default is -1 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestOrderData)
		int32 Index;

	/** Gets a text version of the order */
	FString ToString() const;

	/** operator overloads */
	bool operator==(const FQuestOrderData& Other) const;
	bool operator!=(const FQuestOrderData& Other) const;


};