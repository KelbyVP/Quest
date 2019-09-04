// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestOrder.h"
#include "QuestOrderTargetData.h"

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

EQuestOrderTargetType UQuestOrder::GetTargetType() const
{
	return TargetType;
}

bool UQuestOrder::IsValidTarget(const AActor* OrderedActor, const FQuestOrderTargetData& TargetData) const
{
	return true;
}

EQuestOrderTargetScoringMethod UQuestOrder::GetTargetScoringMethod()
{
	return TargetScoringMethod;
}

EQuestOrderCancellationPolicy UQuestOrder::GetCancellationPolicy()
{
	return CancellationPolicy;
}

float UQuestOrder::GetRange()
{
	return Range;
}

float UQuestOrder::GetTargetAcquisitionRange()
{
	return TargetAcquisitionRange;
}

UBehaviorTree* UQuestOrder::GetBehaviorTree()
{
	return BehaviorTree;
}

bool UQuestOrder::ShouldRestartBehaviorTree()
{
	return bShouldRestartBehaviorTree;
}
