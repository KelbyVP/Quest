// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestOrder.h"

UQuestOrder::UQuestOrder()
{
	TargetType = EQuestOrderTargetType::NONE;
	CancellationPolicy = EQuestOrderCancellationPolicy::CAN_BE_CANCELLED;
	bShouldRestartBehaviorTree = true;
}

void UQuestOrder::GetTagRequirements(const AActor* OrderedActor, FQuestOrderTagRequirements& OutTagRequirements) const
{
	OutTagRequirements = TagRequirements;
}

EQuestOrderTargetType UQuestOrder::GetTargetType()
{
	return TargetType;
}

EQuestOrderCancellationPolicy UQuestOrder::GetCancellationPolicy()
{
	return CancellationPolicy;
}

float UQuestOrder::GetRange()
{
	return Range;
}

UBehaviorTree* UQuestOrder::GetBehaviorTree()
{
	return BehaviorTree;
}

bool UQuestOrder::ShouldRestartBehaviorTree()
{
	return bShouldRestartBehaviorTree;
}
