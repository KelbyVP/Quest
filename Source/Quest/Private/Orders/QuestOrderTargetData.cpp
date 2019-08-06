// Copyright 2019 Kelby Van Patten, All Rights Reserved

#include "QuestOrderTargetData.h"
#include "GameFramework/Actor.h"

FQuestOrderTargetData::FQuestOrderTargetData()
	: Actor(nullptr)
	, Location(FVector::ZeroVector)
	, TargetTags(FGameplayTagContainer::EmptyContainer)
{
}

FQuestOrderTargetData::FQuestOrderTargetData(
	AActor* InActor
	, const FVector InLocation
	, const FGameplayTagContainer& InTargetTags)

	: Actor(InActor)
	, Location(InLocation)
	, TargetTags(InTargetTags)
{
}

FString FQuestOrderTargetData::ToString() const
{
	FString s;
	s += TEXT("(");
	s += TEXT("Actor: ");
	if (Actor != nullptr)
	{
		s += Actor->GetName();
	}

	else
	{
		s += FName().ToString();
	}

	s += TEXT(", Location: ");
	s += Location.ToString();

	s += TEXT(", Tags:( ");
	s += TargetTags.ToString();
	s += TEXT(")");
	s += TEXT(")");

	return s;
}
