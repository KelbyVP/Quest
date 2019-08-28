// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AQuestAIController::AQuestAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AQuestAIController::Tick(float DeltaTime)
{

}

void AQuestAIController::OnPossess(APawn* InPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("QuestAIController::OnPosses: function called!"))


}

