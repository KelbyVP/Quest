// Copyright 2019 Kelby Van Patten, All Rights Reserved

#include "QuestOrderData.h"
#include "GameFramework/Actor.h"
#include "QuestOrder.h"

FQuestOrderData::FQuestOrderData()
{
	OrderType = nullptr;
	bUseLocation = false;
	TargetLocation = FVector::ZeroVector;
	TargetActor = nullptr;
}

FQuestOrderData::FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType)
{
	OrderType = InOrderType;
	bUseLocation = false;
	TargetLocation = FVector::ZeroVector;
	TargetActor = nullptr;
}

FQuestOrderData::FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType, AActor* Actor)
{
	OrderType = InOrderType;
	bUseLocation = false;
	TargetLocation = FVector::ZeroVector;
	TargetActor = Actor;
}

FQuestOrderData::FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType, FVector InTargetLocation)
{
	OrderType = InOrderType;
	bUseLocation = true;
	TargetLocation = InTargetLocation;
	TargetActor = nullptr;
}

FQuestOrderData::FQuestOrderData(TSoftClassPtr<UQuestOrder> InOrderType, AActor* Actor, FVector InTargetLocation)
{
	OrderType = OrderType;
	bUseLocation = true;
	TargetLocation = InTargetLocation;
	TargetActor = Actor;
}

FString FQuestOrderData::ToString() const
{
	FString OrderDataString;

	if (OrderType != nullptr)
	{
		if (!OrderType.IsValid())
		{
			OrderType.LoadSynchronous();
		}
		OrderDataString += OrderType->GetName();
	}
	else
	{
		OrderDataString += TEXT("Unknown Order");
	}
	
	OrderDataString += TEXT(" (");

	if (TargetActor != nullptr)
	{
		OrderDataString += TEXT(", Target: ");
		OrderDataString += TargetActor->GetName();
	}

	if (bUseLocation)
	{
		if (TargetActor != nullptr)
		{
			OrderDataString += TEXT(", Location: ");
			OrderDataString += TargetLocation.ToString();
		}
	}

	OrderDataString += TEXT(")");
	return OrderDataString;
}

bool FQuestOrderData::operator!=(const FQuestOrderData& Other) const
{
	return !(*this == Other);
}

bool FQuestOrderData::operator==(const FQuestOrderData& Other) const
{
	bool bEqual = OrderType == Other.OrderType && bUseLocation == bUseLocation && TargetActor == Other.TargetActor;
	if (bUseLocation)
	{
		bEqual = bEqual && TargetLocation.Equals(Other.TargetLocation);
	}
		return bEqual;
}
