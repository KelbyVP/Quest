// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BehaviorTree/BTNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "QuestBlackboardHelper.generated.h"

/**
 * 
 */
UCLASS(meta = (RestrictedtoClasses = "BTNode"))
class QUEST_API UQuestBlackboardHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static const FName BLACKBOARD_KEY_ORDER_TYPE;
	static const FName BLACKBOARD_KEY_LOCATION;
	static const FName BLACKBOARD_KEY_TARGET;
	static const FName BLACKBOARD_KEY_INDEX;
	static const FName BLACKBOARD_KEY_RANGE;
	static const FName BLACKBOARD_KEY_HOME_LOCATION;
};
