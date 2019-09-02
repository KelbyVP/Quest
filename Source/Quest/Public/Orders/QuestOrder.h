// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestOrderCancellationPolicy.h"
#include "QuestOrderResult.h"
#include "QuestOrderTagRequirements.h"
#include "QuestOrderTargetType.h"
#include "QuestOrder.generated.h"

class UBehaviorTree;

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
	EQuestOrderTargetType GetTargetType();

	/** Gets the order's cancellation policy */
	EQuestOrderCancellationPolicy GetCancellationPolicy();

	/** Gets the order's range */
	float GetRange();

	/** Gets the order's behavior tree */
	UBehaviorTree* GetBehaviorTree();

	/** Tells whether to restart the behavior tree when new order of same type issued */
	bool ShouldRestartBehaviorTree();

	/** The order's tag requirements in order for the character to executed the order */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestOrder", meta = (AllowPrivateAccess = true))
		FQuestOrderTagRequirements TagRequirements;

	/** The type of target that the order acts upon */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestOrder", meta = (AllowPrivateAccess = true))
		EQuestOrderTargetType TargetType;

	/** How the order interacts with other orders */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestOrder", meta = (AllowPrivateAccess = true))
		EQuestOrderCancellationPolicy CancellationPolicy;

	/** Maximum range for targets */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestOrder", meta = (AllowPrivateAccess = true))
		float Range;

	/** The behavior tree associated with the order */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestOrder", meta = (AllowPrivateAccess = true))
		UBehaviorTree* BehaviorTree;

	/** Whether to restart the behavior tree when a new order of same type is issued */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestOrder", meta = (AllowPrivateAccess = true))
		bool bShouldRestartBehaviorTree;


};
