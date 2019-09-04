// Copyright 2019 Kelby Van Patten, All Rights Reserved

#include "QuestOrderTargetData.h"
#include "GameFramework/Actor.h"

FQuestOrderTargetData::FQuestOrderTargetData()
	: Actor(nullptr)
	, Location(FVector::ZeroVector)
	, TargetTags(FGameplayTagContainer::EmptyContainer)
{
}

FQuestOrderTargetData::FQuestOrderTargetData(AActor* InActor, const FVector InLocation, const FGameplayTagContainer& InTargetTags)
	: Actor(InActor)
	, Location(InLocation)
	, TargetTags(InTargetTags)
{
}

FString FQuestOrderTargetData::ToString() const
{
	FString TargetDataString;
	TargetDataString += TEXT("(");
	TargetDataString += TEXT("Actor: ");
	if (Actor != nullptr)
	{
		TargetDataString += Actor->GetName();
	}

	else
	{
		TargetDataString += FName().ToString();
	}

	TargetDataString += TEXT(", Location: ");
	TargetDataString += Location.ToString();

	TargetDataString += TEXT(", Tags:( ");
	TargetDataString += TargetTags.ToString();
	TargetDataString += TEXT(")");
	TargetDataString += TEXT(")");

	return TargetDataString;
}
