// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestOrder.h"
#include "QuestOrderProcessPolicy.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "QuestAIController.h"
#include "QuestOrderData.h"
#include "QuestGlobalTags.h"

UQuestOrder::UQuestOrder()
{
	OrderProcessPolicy = EQuestOrderProcessPolicy::CAN_BE_CANCELLED;
	TagRequirements.SourceRequiredTags.AddTag(UQuestGlobalTags::Status_Alive());
	TargetType = EQuestOrderTargetType::NONE;
}

bool UQuestOrder::CanObeyOrder(const AActor* OrderedActor, int32 Index, FQuestOrderErrorTags* OutErrorTags /*= nullptr*/) const
{
	return true;
}

bool UQuestOrder::IsValidTarget(const AActor* OrderedActor, const FQuestOrderTargetData& TargetData, int32 Index, FQuestOrderErrorTags* OutErrorTags /*= nullptr*/) const
{
	return true;
}

UBehaviorTree* UQuestOrder::GetBehaviorTree() const
{
	return BehaviorTree;
}

EQuestOrderProcessPolicy UQuestOrder::GetOrderProcessPolicy(const AActor* OrderedActor, int32 Index) const
{
	return OrderProcessPolicy;
}

void UQuestOrder::GetTagRequirements(const AActor* OrderedActor, int32 Index, FQuestOrderTagRequirements& OutTagRequirements) const
{
	OutTagRequirements = TagRequirements;
}

void UQuestOrder::GetSuccessTagRequirements(const AActor* OrderedActor, int32 Index, FQuestOrderTagRequirements& OutTagRequirements) const
{
	OutTagRequirements = SuccessTagRequirements;
}

bool UQuestOrder::ShouldRestartBehaviorTree() const
{
	return bShouldRestartBehaviorTree;
}

void UQuestOrder::IssueOrder(AActor* OrderedActor, const FQuestOrderTargetData& TargetData, int32 Index, 
	FQuestOrderCallback Callback, const FVector& HomeLocation) const
{
	/** Check whether actor and controller are valid */
	if (!IsValid(OrderedActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Ordered actor is invalid."));
		Callback.Broadcast(EQuestOrderResult::FAILED);
		return;
	}

	APawn* Pawn = Cast<APawn>(OrderedActor);
	if (Pawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("The ordered actor '%s' is not a pawn."), *OrderedActor->GetName());
		Callback.Broadcast(EQuestOrderResult::FAILED);
		return;
	}

	AQuestAIController* Controller = Cast<AQuestAIController>(Pawn->GetController());
	if (Controller == nullptr)
	{
		UE_LOG(
			LogTemp, 
			Warning, 
			TEXT("The pawn % does not have a controller of type QuestAIController and therefore cannot receive orders."), 
			*OrderedActor->GetName());
		Callback.Broadcast(EQuestOrderResult::FAILED);
		return;
	}

	AActor* TargetActor = TargetData.Actor;
	const FVector TargetLocation = TargetData.Location;

	FQuestOrderData Order(GetClass(), Index, TargetActor, TargetLocation);
	//EQuestOrderTargetType TargetType = GetTargetType(OrderedActor, Index);

	Order.bUseLocation = TargetType == EQuestOrderTargetType::LOCATION || TargetType == EQuestOrderTargetType::DIRECTION;
	Controller->IssueOrder(Order, Callback, HomeLocation);
}

void UQuestOrder::OrderCancelled(AActor* OrderedActor, const FQuestOrderTargetData& TargetData, int32 Index) const
{

}

EQuestOrderTargetType UQuestOrder::GetTargetType(const AActor* OrderedActor, int32 Index) const
{
	return TargetType;
}
