// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QuestOrderErrorTags.h"
#include "QuestOrderHelperLibrary.generated.h"

class UQuestOrder;

/**
 * 
 */
UCLASS()
class QUEST_API UQuestOrderHelperLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Checks whether the ordered actor can obey the order */
	UFUNCTION(BlueprintPure, Category = "QuestOrderHelperLibrary")
		static bool CanObeyOrder(TSoftClassPtr<UQuestOrder> OrderType, const AActor* OrderedActor);

	/** Gets an order's target type */
	static EQuestOrderTargetType GetTargetType(TSoftClassPtr<UQuestOrder> OrderType);

	/** Gets an order's cancellation policy */
	static EQuestOrderCancellationPolicy GetCancellationPolicy(TSoftClassPtr<UQuestOrder> OrderType);
};
