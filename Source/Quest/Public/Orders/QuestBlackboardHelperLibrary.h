// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BehaviorTree/BTNode.h"
#include "QuestOrder.h"
#include "QuestBlackboardHelperLibrary.generated.h"

/**
 * 
 */
UCLASS()
class QUEST_API UQuestBlackboardHelperLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "QuestBlackboardHelperLibrary", meta = (HidePin = "NodeOwner", DefaultToSelf = "NodeOwner"))
		static TSubclassOf<UQuestOrder> GetBlackboardOrderType(UBTNode* NodeOwner);
	UFUNCTION(BlueprintPure, Category = "QuestBlackboardHelperLibrary", meta = (HidePin = "NodeOwner", DefaultToSelf = "NodeOwner"))
		static FVector GetBlackboardLocation(UBTNode* NodeOwner);
	UFUNCTION(BlueprintPure, Category = "QuestBlackboardHelperLibrary", meta = (HidePin = "NodeOwner", DefaultToSelf = "NodeOwner"))
		static AActor* GetBlackboardTarget(UBTNode* NodeOwner);
	UFUNCTION(BlueprintPure, Category = "QuestBlackboardHelperLibrary", meta = (HidePin = "NodeOwner", DefaultToSelf = "NodeOwner"))
		static float GetBlackboardRange(UBTNode* NodeOwner);

	static const FName BLACKBOARD_KEY_ORDER_TYPE;
	static const FName BLACKBOARD_KEY_TARGET;
	static const FName BLACKBOARD_KEY_LOCATION;
	static const FName BLACKBOARD_KEY_RANGE;
};
