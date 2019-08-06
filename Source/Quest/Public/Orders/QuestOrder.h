// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestOrderResult.h"
#include "QuestOrderProcessPolicy.h"
#include "QuestOrderTagRequirements.h"
#include "QuestOrderTargetData.h"
#include "QuestOrderTargetType.h"
#include "QuestOrder.generated.h"

struct FQuestOrderErrorTags;
class UBehaviorTree;

/**  Delegate that broadcasts when an order is complete */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestOrderCallback, EQuestOrderResult, Result);

/**
 * Type of order to issue to characters
 */
UCLASS(BlueprintType, Abstract, Blueprintable, Const)
class QUEST_API UQuestOrder : public UObject
{
	GENERATED_BODY()
	
public:
	UQuestOrder();

	/** Tells us whether this character can obey this order */
	virtual bool CanObeyOrder(const AActor* OrderedActor, int32 Index, FQuestOrderErrorTags* OutErrorTags = nullptr) const;

	/** Whether the target actor and/or location are valid */
	virtual bool IsValidTarget(const AActor* OrderedActor, const FQuestOrderTargetData& TargetData, int32 Index,
		FQuestOrderErrorTags* OutErrorTags = nullptr) const;

	/** Gets the behavior tree associated with this order */
	UBehaviorTree* GetBehaviorTree() const;

	/** Gets a value that describes how order is executed.  */
	virtual EQuestOrderProcessPolicy GetOrderProcessPolicy(const AActor* OrderedActor, int32 Index) const;

	/** Gets tags necessary to issue order */
	virtual void GetTagRequirements(const AActor* OrderedActor, int32 Index, FQuestOrderTagRequirements& OutTagRequirements) const;

	/** Gets tags necessary for order to be successful */
	virtual void GetSuccessTagRequirements(const AActor* OrderedActor, int32 Index, 
		FQuestOrderTagRequirements& OutTagRequirements) const;

	/** Tells whether to restart the behavior tree when order issued */
	bool ShouldRestartBehaviorTree() const;

	/** Start the order */
	virtual void IssueOrder(AActor* OrderedActor, const FQuestOrderTargetData& TargetData, int32 Index, FQuestOrderCallback Callback, const FVector& HomeLocation) const;

	virtual void OrderCancelled(AActor* OrderedActor, const FQuestOrderTargetData& TargetData, int32 Index) const;

	/** Returns the target type that this order is using */
	virtual EQuestOrderTargetType GetTargetType(const AActor* OrderedActor, int32 Index) const;

protected:
	//	Describes how an order is executed, determines how displayed in UI, and how order is handled by order system
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "QuestOrder")
		EQuestOrderProcessPolicy OrderProcessPolicy;

	//	Tag requirements that must be present for the order to issue
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "QuestOrder")
		FQuestOrderTagRequirements TagRequirements;

	// Tag requirements that must be present for the order to be successful
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "QuestOrder")
		FQuestOrderTagRequirements SuccessTagRequirements;

	// Target type for this order
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "QuestOrder")
		EQuestOrderTargetType TargetType;

private:

	/** The behavior tree that runs when this order is executed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		UBehaviorTree* BehaviorTree;

	/** Whether to restart the behavior tree */
	bool bShouldRestartBehaviorTree;

};
