// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QuestOrderErrorTags.h"
#include "QuestOrderTargetData.h"
#include "QuestOrderHelper.generated.h"


class UQuestOrder;
class UBehaviorTree;
/**
 * 
 */
UCLASS()
class QUEST_API UQuestOrderHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/** Whether the actor can obey this type of order */
	UFUNCTION(BlueprintPure, Category = QuestOrderHelper)
		static bool CanObeyOrder(TSoftClassPtr<UQuestOrder> OrderType, const AActor* OrderedActor, int32 Index = -1);
		static bool CanObeyOrder(TSoftClassPtr<UQuestOrder> OrderType, const AActor* OrderedActor, int32 Index, FQuestOrderErrorTags* OutErrorTags);

	UFUNCTION(BlueprintPure, Category = "QuestOrder")
		static EQuestOrderTargetType GetTargetType(TSoftClassPtr<UQuestOrder> OrderType, 
			const AActor* OrderedActor = nullptr, int32 Index = -1);

	/** Whether order is successful */
	UFUNCTION(BlueprintPure, Category = "QuestOrder")
		static bool CanOrderBeConsideredAsSucceeded(TSoftClassPtr<UQuestOrder> OrderType, const AActor* OrderedActor,
			const FQuestOrderTargetData& TargetData, int32 Index = -1);

	/** Whether the target actor or location is valid */
	UFUNCTION(BlueprintPure, Category = "QuestOrderHelper")
		static bool IsValidTarget(TSoftClassPtr<UQuestOrder> OrderType, const AActor* OrderedActor,
			const FQuestOrderTargetData& TargetData, int32 Index = -1);

		static bool IsValidTarget(TSoftClassPtr<UQuestOrder> OrderType, const AActor* OrderedActor,
			const FQuestOrderTargetData& TargetData, int32 Index, 
			FQuestOrderErrorTags* OutErrorTags);


	/** Creates order target data */
	UFUNCTION(BlueprintPure, Category = QuestOrderHelper)
		static FQuestOrderTargetData CreateOrderTargetData(const AActor* OrderedActor, AActor* TargetActor,
			const FVector& TargetLocation = FVector::ZeroVector);

	/** Gets the behavior tree that runs when this order executes */
	UFUNCTION(BlueprintPure, Category = QuestOrderHelper)
		static UBehaviorTree* GetBehaviorTree(TSoftClassPtr<UQuestOrder> OrderType);

	/** Gets value that describes how order is executed.  */
	UFUNCTION(BlueprintPure, Category = "QuestOrderHelper")
		static EQuestOrderProcessPolicy GetOrderProcessPolicy(TSoftClassPtr<UQuestOrder> OrderType, 
			const AActor* OrderedActor, int32 Index = -1);

	/**
	 * Gets the tag requirements for this order to be executed
	 */
	UFUNCTION(BlueprintPure, Category = "QuestOrderHelper")
		static void GetOrderTagRequirements(TSoftClassPtr<UQuestOrder> OrderType, 
			const AActor* OrderedActor, int32 Index, FQuestOrderTagRequirements& OutTagRequirements);

	/** Tells us whether to restart the tree if a new order of the same type is issued */
	static bool ShouldRestartBehaviorTree(TSoftClassPtr<UQuestOrder> OrderType);
};
