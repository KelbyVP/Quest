// Copyright 2019 Kelby Van Patten, All Rights Reserved

#include "QuestOrderData.h"
#include "GameFramework/Actor.h"
#include "QuestOrder.h"

FQuestOrderData::FQuestOrderData()
{
	OrderType = nullptr;
	bUseLocation = false;
	Location = FVector::ZeroVector;
	Target = nullptr;
	Index = -1;
}

FQuestOrderData::FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType)
{
	OrderType = InOrderType;
	bUseLocation = false;
	Location = FVector::ZeroVector;
	Target = nullptr;
	Index = -1;
}

FQuestOrderData::FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType, AActor* InTarget)
{
	OrderType = InOrderType;
	bUseLocation = false;
	Location = FVector::ZeroVector;
	Target = InTarget;
	Index = -1;
}


FQuestOrderData::FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType, FVector InLocation)
{
	OrderType = InOrderType;
	bUseLocation = true;
	Location = InLocation;
	Target = nullptr;
	Index = -1;
}

FQuestOrderData::FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType, int32 InIndex)
{
	OrderType = InOrderType;
	bUseLocation = false;
	Location = FVector::ZeroVector;
	Target = nullptr;
	Index = InIndex;
}

FQuestOrderData::FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType, AActor* InTarget, FVector InLocation)
{
	OrderType = InOrderType;
	bUseLocation = true;
	Location = InLocation;
	Target = InTarget;
	Index = -1;
}

FQuestOrderData::FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType, int32 InIndex, AActor* InTarget)
{
	OrderType = InOrderType;
	bUseLocation = false;
	Location = FVector::ZeroVector;
	Target = InTarget;
	Index = InIndex;
}

FQuestOrderData::FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType, int32 InIndex, FVector InLocation)
{
	OrderType = InOrderType;
	bUseLocation = true;
	Location = InLocation;
	Target = nullptr;
	Index = InIndex;
}

FQuestOrderData::FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType, int32 InIndex, AActor* InTarget, FVector InLocation)
{
	OrderType = InOrderType;
	bUseLocation = true;
	Location = InLocation;
	Target = InTarget;
	Index = InIndex;
}

FString FQuestOrderData::ToString() const
{
	FString OrderAsString;

	if (OrderType != nullptr)
	{
		if (!OrderType.IsValid())
		{
			OrderType.LoadSynchronous();
		}

		OrderAsString += OrderType->GetName();
	}
	else
	{
		OrderAsString += TEXT("Unknown Order");
	}

	OrderAsString += TEXT(" (");

	if (Index >= 0)
	{
		OrderAsString += TEXT("Index: ");
		OrderAsString += FString::FromInt(Index);
	}

	if (Target != nullptr)
	{
		if (Index >= 0)
		{
			OrderAsString += TEXT(", Target: ");
		}

		else
		{
			OrderAsString += TEXT("Target: ");
		}

		OrderAsString += Target->GetName();
	}

	if (bUseLocation)
	{
		if (Target != nullptr || Index >= 0)
		{
			OrderAsString += TEXT(", Location: ");
		}

		else
		{
			OrderAsString += TEXT("Location: ");
		}

		OrderAsString += Location.ToString();
	}

	OrderAsString += TEXT(")");

	return OrderAsString;
}

bool FQuestOrderData::operator==(const FQuestOrderData& Other) const
{
	bool bEqual =
		OrderType == Other.OrderType && bUseLocation == bUseLocation && Target == Other.Target && Index == Other.Index;

	if (bUseLocation)
	{
		bEqual = bEqual && Location.Equals(Other.Location);
	}

	return bEqual;
}

bool FQuestOrderData::operator!=(const FQuestOrderData& Other) const
{
	return !(*this == Other);
}

