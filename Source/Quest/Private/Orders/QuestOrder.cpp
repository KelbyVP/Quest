// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestOrder.h"
#include "GameFramework/Actor.h"
#include "QuestAIController.h"
#include "QuestOrderData.h"
#include "QuestOrderTargetData.h"
#include "QuestOrderTargetType.h"

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

void UQuestOrder::IssueOrder(AActor* OrderedActor, const FQuestOrderTargetData& TargetData, FQuestOrderCallback Callback, const TSubclassOf<UQuestGameplayAbility> Ability) const
{
	if (!IsValid(OrderedActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestOrder::IssueOrder: Ordered Actor is invalid!"));
		Callback.Broadcast(EQuestOrderResult::FAILED);
		return;
	}

	APawn* Pawn = Cast<APawn>(OrderedActor);
	if (Pawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestOrder::IssueOrder: Ordered Actor % is not a pawn!"), *OrderedActor->GetName());
		Callback.Broadcast(EQuestOrderResult::FAILED);
		return;
	}

	AQuestAIController* Controller = Cast<AQuestAIController>(Pawn->GetController());
	if (Controller == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestOrder::IssueOrder: Ordered Actor % does not have the required controller for order %s!"), *OrderedActor->GetName(), *GetName());
		Callback.Broadcast(EQuestOrderResult::FAILED);
		return;
	}

	AActor* TargetActor = TargetData.Actor;
	const FVector TargetLocation = TargetData.Location;

	UClass* OrderType = GetClass();
	if (OrderType == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestOrder::IssueOrder: OrderType is a null pointer!"));
		Callback.Broadcast(EQuestOrderResult::FAILED);
		return;
	}
	FQuestOrderData Order(OrderType, TargetActor, TargetLocation);
	EQuestOrderTargetType OrderTargetType = GetTargetType();

	Order.bUseLocation = OrderTargetType == EQuestOrderTargetType::LOCATION || OrderTargetType == EQuestOrderTargetType::DIRECTION;
	Order.Ability = Ability;
	if (IsValid(Controller))
	{
		Controller->IssueOrder(Order, Callback);
		return;
	}
	
}

bool UQuestOrder::ShouldRestartBehaviorTree()
{
	return bShouldRestartBehaviorTree;
}
