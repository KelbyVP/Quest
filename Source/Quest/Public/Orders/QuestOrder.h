// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestOrderCancellationPolicy.h"
#include "QuestOrderTagRequirements.h"
#include "QuestOrderTargetType.h"
#include "QuestOrder.generated.h"


/**
 * An order type that can be given to a character to execute
 */
UCLASS()
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

	/** The order's tag requirements in order for the character to executed the order */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestOrder", meta = (AllowPrivateAccess = true))
		FQuestOrderTagRequirements TagRequirements;

	/** The type of target that the order acts upon */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestOrder", meta = (AllowPrivateAccess = true))
		EQuestOrderTargetType TargetType;

	///** How the order interacts with other orders */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestOrder", meta = (AllowPrivateAccess = true))
		EQuestOrderCancellationPolicy CancellationPolicy;

};
