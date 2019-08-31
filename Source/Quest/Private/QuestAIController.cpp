// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "QuestOrderData.h"

AQuestAIController::AQuestAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AQuestAIController::Tick(float DeltaTime)
{

}

void AQuestAIController::OnPossess(APawn* InPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("QuestAIController::OnPossess:: function called!"));
}

void AQuestAIController::SetBlackboardValues(const FQuestOrderData& Order)
{
	if (!VerifyBlackboard())
	{
		return;
	}

	//  TODO:  Set blackboard values (see OrdersAbilities function)


}

bool AQuestAIController::VerifyBlackboard() const
{
	if (!Blackboard)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestAIController::VerifyBlackboard:: No blackboard for %s"), *GetPawn()->GetName());
		return false;
	}
	return true;
}

