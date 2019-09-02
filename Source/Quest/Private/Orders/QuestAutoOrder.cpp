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

bool FQuestAutoOrder::FindAutoOrderInArray(const TArray<FQuestAutoOrder> InArray, int32& Index)
{
	if (InArray.Num() > 0)
	{
		for (int i = 0; i < InArray.Num(); i++)
		{
			if (OrderType == InArray[i].OrderType &&
				bHasBeenUsed == InArray[i].bHasBeenUsed)
			{
				Index = i;
				return true;
			}
		}
	}
	return false;
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
