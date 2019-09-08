// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestBlackboardHelperLibrary.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"

const FName UQuestBlackboardHelperLibrary::BLACKBOARD_KEY_ORDER_TYPE = TEXT("Order_OrderType");
const FName UQuestBlackboardHelperLibrary::BLACKBOARD_KEY_LOCATION = TEXT("Order_Location");
const FName UQuestBlackboardHelperLibrary::BLACKBOARD_KEY_TARGET = TEXT("Order_Target");
const FName UQuestBlackboardHelperLibrary::BLACKBOARD_KEY_RANGE = TEXT("Order_Range");

TSubclassOf<UQuestOrder> UQuestBlackboardHelperLibrary::GetBlackboardOrderType(UBTNode* NodeOwner)
{
	UBlackboardComponent* BlackboardComp = UBTFunctionLibrary::GetOwnersBlackboard(NodeOwner);
	return BlackboardComp ? BlackboardComp->GetValueAsClass(BLACKBOARD_KEY_ORDER_TYPE) : nullptr;
}

FVector UQuestBlackboardHelperLibrary::GetBlackboardLocation(UBTNode* NodeOwner)
{
	UBlackboardComponent* BlackboardComp = UBTFunctionLibrary::GetOwnersBlackboard(NodeOwner);
	return BlackboardComp ? BlackboardComp->GetValueAsVector(BLACKBOARD_KEY_LOCATION) : FVector::ZeroVector;
}

AActor* UQuestBlackboardHelperLibrary::GetBlackboardTarget(UBTNode* NodeOwner)
{
	UBlackboardComponent* BlackboardComp = UBTFunctionLibrary::GetOwnersBlackboard(NodeOwner);
	return BlackboardComp ? Cast<AActor>(BlackboardComp->GetValueAsObject(BLACKBOARD_KEY_TARGET)) : nullptr;
}

float UQuestBlackboardHelperLibrary::GetBlackboardRange(UBTNode* NodeOwner)
{
	UBlackboardComponent* BlackboardComp = UBTFunctionLibrary::GetOwnersBlackboard(NodeOwner);
	return BlackboardComp ? BlackboardComp->GetValueAsFloat(BLACKBOARD_KEY_RANGE) : 0.0f;
}
