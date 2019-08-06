// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestBlackboardHelper.h"

#include "BehaviorTree/BTFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"

const FName UQuestBlackboardHelper::BLACKBOARD_KEY_ORDER_TYPE = TEXT("Order_OrderType");
const FName UQuestBlackboardHelper::BLACKBOARD_KEY_LOCATION = TEXT("Order_Location");
const FName UQuestBlackboardHelper::BLACKBOARD_KEY_TARGET = TEXT("Order_Target");
const FName UQuestBlackboardHelper::BLACKBOARD_KEY_INDEX = TEXT("Order_Index");
const FName UQuestBlackboardHelper::BLACKBOARD_KEY_RANGE = TEXT("Order_Range");
const FName UQuestBlackboardHelper::BLACKBOARD_KEY_HOME_LOCATION = TEXT("Order_HomeLocation");
