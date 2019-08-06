// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "QuestBlackboardHelper.h"
#include "QuestOrderHelper.h"

AQuestAIController::AQuestAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AQuestAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Blackboard == nullptr)
	{
		return;
	}

	switch (BehaviorTreeResult)
	{
	case EBTNodeResult::InProgress:
		break;
	case EBTNodeResult::Failed:
		CurrentOrderResultCallback.Broadcast(EQuestOrderResult::FAILED);
		break;
	case EBTNodeResult::Aborted:
		break;
	case EBTNodeResult::Succeeded:
		CurrentOrderResultCallback.Broadcast(EQuestOrderResult::SUCCEEDED);
		break;
	}
	BehaviorTreeResult = EBTNodeResult::InProgress;
}

bool AQuestAIController::HasOrder(TSubclassOf<UQuestOrder> OrderType) const
{
	if (!VerifyBlackboard())
	{
		return false;
	}
	return Blackboard->GetValueAsClass(UQuestBlackboardHelper::BLACKBOARD_KEY_ORDER_TYPE) == OrderType;
}

TSoftClassPtr<UQuestOrder> AQuestAIController::GetStopOrder() const
{
	return StopOrder;
}

void AQuestAIController::BehaviorTreeEnded(EBTNodeResult::Type Result)
{
	if (!VerifyBlackboard())
	{
		return;
	}

	switch (Result)
	{
	case EBTNodeResult::InProgress:
		break;
	case EBTNodeResult::Failed:
		BehaviorTreeResult = EBTNodeResult::Failed;
		break;
	case EBTNodeResult::Aborted:
		break;
	case EBTNodeResult::Succeeded:
		BehaviorTreeResult = EBTNodeResult::Succeeded;
		break;
	}
}

void AQuestAIController::IssueOrder(const FQuestOrderData& Order, FQuestOrderCallback Callback, const FVector& HomeLocation)
{
		UBehaviorTree* BehaviorTree = UQuestOrderHelper::GetBehaviorTree(Order.OrderType.Get());
		
		if (BehaviorTree == nullptr)
		{
			Callback.Broadcast(EQuestOrderResult::FAILED);
			return;
		}

		CurrentOrderResultCallback = Callback;
		BehaviorTreeResult = EBTNodeResult::InProgress;

		SetBlackboardValues(Order, HomeLocation);

		// Stop any open orders and start again
		ApplyOrder(Order, BehaviorTree);
}

void AQuestAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Load assets
	StopOrder.LoadSynchronous();

	// Give AI assigned blackboard
	UBlackboardComponent* BlackboardComponent;
	if (UseBlackboard(CharacterBlackboardAsset, BlackboardComponent))
	{
		// Setup blackboard
		SetBlackboardValues(FQuestOrderData(StopOrder.Get()), InPawn->GetActorLocation());
	}

	// Call RunBehaviorTree.  This will set up the behavior tree component.
	UBehaviorTree* BehaviorTree = UQuestOrderHelper::GetBehaviorTree(StopOrder.Get());
	RunBehaviorTree(BehaviorTree);
}

bool AQuestAIController::VerifyBlackboard() const
{
	if (!Blackboard)
	{
		UE_LOG(
			LogTemp, Warning, 
			TEXT("Blackboard not set up for %s.  Check AI Controller class and Auto Possess AI"), 
			*GetPawn()->GetName());
		return false;
	}
	return true;
}

void AQuestAIController::SetBlackboardValues(const FQuestOrderData& Order, const FVector& HomeLocation)
{
	if (!VerifyBlackboard())
	{
		return;
	}

	Blackboard->SetValueAsClass(UQuestBlackboardHelper::BLACKBOARD_KEY_ORDER_TYPE, Order.OrderType.Get());
	if (Order.bUseLocation)
	{
		Blackboard->SetValueAsVector(UQuestBlackboardHelper::BLACKBOARD_KEY_LOCATION, FVector(Order.Location));
	}
}

void AQuestAIController::ApplyOrder(const FQuestOrderData& Order, UBehaviorTree* BehaviorTree)
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent != nullptr && BehaviorTree != nullptr)
	{
		/** If it's the same tree, restart the tree */
		UBehaviorTree* CurrentTree = BehaviorTreeComponent->GetRootTree();
		if (CurrentTree == BehaviorTree)
		{
			if (UQuestOrderHelper::ShouldRestartBehaviorTree(Order.OrderType.Get()))
			{
				BehaviorTreeComponent->RestartTree();
			}
		}
		else
		{
			BehaviorTreeComponent->StartTree(*BehaviorTree, EBTExecutionMode::SingleRun);
		}
	}
}
