// Copyright 2019 Kelby Van Patten, All Rights Reserved

#include "QuestAutoOrder.h"

/**
 * Sets default values
 */

FQuestAutoOrder::FQuestAutoOrder()
{
	bHasBeenUsed = false;
}

FQuestAutoOrder::FQuestAutoOrder(TSoftClassPtr<UQuestOrder> InOrderType)
{
	OrderType = InOrderType;
	bHasBeenUsed = false;
}

bool FQuestAutoOrder::operator!=(const FQuestAutoOrder& Other) const
{
	return !(*this == Other);
}

bool FQuestAutoOrder::operator==(const FQuestAutoOrder& Other) const
{
	bool bEqual =
		OrderType == Other.OrderType &&
		bHasBeenUsed == Other.bHasBeenUsed;
	return bEqual;
}
