// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "QuestOrder.h"
#include "QuestAutoOrder.generated.h"

/** 
*	A struct that has an order and a bool that tells whether the order has already been used.
*	Used in tracking whether auto orders have already been used.
*/

USTRUCT(BlueprintType)
struct QUEST_API FQuestAutoOrder
{
	GENERATED_BODY()

	/** Constructors */
	FQuestAutoOrder();
	FQuestAutoOrder(TSoftClassPtr<UQuestOrder> InOrderType);

	/** The order type */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestAutoOrder")
		TSoftClassPtr<UQuestOrder> OrderType;

	/** Whether the order has already been used */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestAutoOrder")
		bool bHasBeenUsed;

	/** Operator overloads */
	bool operator==(const FQuestAutoOrder& Other) const;
	bool operator!=(const FQuestAutoOrder& Other) const;
};