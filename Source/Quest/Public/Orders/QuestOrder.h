// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestOrderCancellationPolicy.h"
#include "QuestOrderResult.h"
#include "QuestOrderTagRequirements.h"
#include "QuestOrderTargetScoringMethod.h"
#include "QuestOrderTargetType.h"
#include "QuestOrder.generated.h"

struct FQuestOrderTargetData;
class UBehaviorTree;
class UQuestGameplayAbility;

/** Delegate used to broadcast how the order finished */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestOrderCallback, EQuestOrderResult, Result);

/**
 * An order type that can be given to a character to execute
 */
UCLASS(BlueprintType, Abstract, Blueprintable, const)
class QUEST_API UQuestOrder : public UObject
{
	GENERATED_BODY()
	

public:

	UQuestOrder();

	/** Gets the order's tag requirements */
	virtual void GetTagRequirements(const AActor* OrderedActor, FQuestOrderTagRequirements& OutTagRequirements) const;

	/** Gets the order's target type */
	EQuestOrderTargetType GetTargetType() const;

	/** Checks any unique targeting requirements for this order; should be overriden by child classes if there are unique requirements */
	virtual bool IsValidTarget(const AActor* OrderedActor, const FQuestOrderTargetData& TargetData) const;

	/** Gets the order's target scoring method */
	EQuestOrderTargetScoringMethod GetTargetScoringMethod();

	/** Gets the order's cancellation policy */
	EQuestOrderCancellationPolicy GetCancellationPolicy();

	/** Gets the order's range */
	float GetRange();

	/** Gets the order's target acquisition range */
	float GetTargetAcquisitionRange();

	/** Gets the order's behavior tree */
	UBehaviorTree* GetBehaviorTree();

	/** Starts the order.  Callback must be called when the character finishes executing the order. */
	virtual void IssueOrder(AActor* OrderedActor, const FQuestOrderTargetData& TargetData, FQuestOrderCallback Callback, const TSubclassOf<UQuestGameplayAbility> Ability) const;

	/** Tells whether to restart the behavior tree when new order of same type issued */
	bool ShouldRestartBehaviorTree();

	/** The order's tag requirements in order for the character to executed the order */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestOrder", meta = (AllowPrivateAccess = true))
		FQuestOrderTagRequirements TagRequirements;

	/** The type of target that the order acts upon */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestOrder", meta = (AllowPrivateAccess = true))
		EQuestOrderTargetType TargetType;

	/** The method used to choose a target */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestOrder", meta = (AllowPrivateAccess = true))
		EQuestOrderTargetScoringMethod TargetScoringMethod;

	/** How the order interacts with other orders */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestOrder", meta = (AllowPrivateAccess = true))
		EQuestOrderCancellationPolicy CancellationPolicy;

	/** Maximum range that can be reached by the order */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestOrder", meta = (AllowPrivateAccess = true))
		float Range;

	/** Maximum range to look for targets; may be larger than Range if order allows character to move toward targets */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestOrder", meta = (AllowPrivateAccess = true))
		float TargetAcquisitionRange;

	/** The behavior tree associated with the order */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestOrder", meta = (AllowPrivateAccess = true))
		UBehaviorTree* BehaviorTree;

	/** Whether to restart the behavior tree when a new order of same type is issued */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestOrder", meta = (AllowPrivateAccess = true))
		bool bShouldRestartBehaviorTree;


};
