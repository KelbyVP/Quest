// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "QuestAutoOrderComponent.h"
#include "QuestBlackboardHelperLibrary.h"
#include "QuestCharacterBase.h"
#include "QuestCharacterGroup.h"
#include "QuestOrder.h"
#include "QuestOrderHelperLibrary.h"
#include "QuestDefaultOrder.h"
#include "QuestCharacterBase.h"
#include "UObjectGlobals.h"

AQuestAIController::AQuestAIController()
{
	/** Create AI Perception Component and configuration */

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	/** Detect all characters  */
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	/** Set dominant sense as sight */
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());

	/** Function called when perception component perceives something */
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AQuestAIController::OnPawnDetected);

	GetPerceptionComponent()->ConfigureSense(*SightConfig);

	PrimaryActorTick.bCanEverTick = true;
}

void AQuestAIController::Tick(float DeltaTime)
{
}

void AQuestAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	TSoftClassPtr<UQuestOrder> DefaultOrder;
	AQuestCharacterBase* ControlledPawn = Cast<AQuestCharacterBase>(GetPawn());

	/** Updates the PerceptionComponent's variables based on the character settings */
	InitializePerceptionComponent(ControlledPawn);

	/** Set the default order */
	if (ControlledPawn)
	{
		DefaultOrder = ControlledPawn->GetDefaultOrder();
	}
	DefaultOrder.LoadSynchronous();

	/** Set up the blackboard */
	UBlackboardComponent* BlackboardComponent;
	if (UseBlackboard(CharacterBlackboard, BlackboardComponent))
	{
		SetBlackboardValues(FQuestOrderData(DefaultOrder.Get()));
	}
	UBehaviorTree* BehaviorTree = UQuestOrderHelperLibrary::GetBehaviorTree(DefaultOrder.Get());
	RunBehaviorTree(BehaviorTree);
}


void AQuestAIController::InitializePerceptionComponent(AQuestCharacterBase* ControlledPawn)
{
	/** Set the perception component to use the controlled pawn's settings */

	if (ControlledPawn && SightConfig)
	{
		SightConfig->SightRadius = ControlledPawn->AISightRadius;
		SightConfig->LoseSightRadius = ControlledPawn->AILoseSightRadius;
		SightConfig->PeripheralVisionAngleDegrees = ControlledPawn->AIFieldOfView;
		SightConfig->SetMaxAge(ControlledPawn->AISightAge);

		/** Detect all types of characters */
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

void AQuestAIController::IssueOrder(const FQuestOrderData& Order)
{
	UBehaviorTree* BehaviorTree = UQuestOrderHelperLibrary::GetBehaviorTree(Order.OrderType.Get());
	if (BehaviorTree == nullptr)
	{
		return;
	}

	SetBlackboardValues(Order);
	ApplyOrder(Order, BehaviorTree);
}


bool AQuestAIController::VerifyBlackboard() const
{
	if (!Blackboard)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestAIController:VerifyBlackboard: No blackboard found for %s!"), *GetPawn()->GetName());
		return false;
	}
	return true;
}

void AQuestAIController::SetBlackboardValues(const FQuestOrderData& Order)
{
	if (!VerifyBlackboard())
	{
		return;
	}

	/** Set target location */
	Blackboard->SetValueAsClass(UQuestBlackboardHelperLibrary::BLACKBOARD_KEY_ORDER_TYPE, Order.OrderType.Get());
	if (Order.bUseLocation)
	{
		Blackboard->SetValueAsVector(UQuestBlackboardHelperLibrary::BLACKBOARD_KEY_LOCATION, 
			FVector(Order.TargetLocation.X, Order.TargetLocation.Y, 0.0f));
	}
	else
	{
		Blackboard->ClearValue(UQuestBlackboardHelperLibrary::BLACKBOARD_KEY_LOCATION);
	}

	Blackboard->SetValueAsObject(UQuestBlackboardHelperLibrary::BLACKBOARD_KEY_TARGET, Order.TargetActor);
	Blackboard->SetValueAsFloat(UQuestBlackboardHelperLibrary::BLACKBOARD_KEY_RANGE,
		UQuestOrderHelperLibrary::GetRange(Order.OrderType));
}

void AQuestAIController::ApplyOrder(const FQuestOrderData& Order, UBehaviorTree* BehaviorTree)
{
	UE_LOG(LogTemp, Warning, TEXT("QuestAIController::ApplyOrder: Apply order called!"))
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestAIController::ApplyOrder: no behavior tree component found!"))
	}
	if (BehaviorTree == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestAIController::ApplyOrder: no behavior tree found!"))
	}
	if (BehaviorTreeComponent != nullptr && BehaviorTree != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestAIController::applyOrder: not null pointers!"))
		/** If the tree is the same, restart it if it should restart*/
		UBehaviorTree* CurrentTree = BehaviorTreeComponent->GetRootTree();
		if (CurrentTree == BehaviorTree)
		{
			if (UQuestOrderHelperLibrary::ShouldRestartBehaviorTree(Order.OrderType.Get()))
			{
				BehaviorTreeComponent->RestartTree();
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("QuestAIController::ApplyOrder: starting tree!"))
			BehaviorTreeComponent->StartTree(*BehaviorTree, EBTExecutionMode::SingleRun);
		}
	}
}

void AQuestAIController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	AQuestCharacterBase* ControlledCharacter = Cast<AQuestCharacterBase>(GetPawn());
	for (auto& PawnToCheck : DetectedPawns)
	{
		AQuestCharacterBase* CharacterDetected = Cast<AQuestCharacterBase>(PawnToCheck);
		if (CharacterDetected)
		{
				ControlledCharacter->CharacterGroup->CheckShouldStartFighting(CharacterDetected);
		}
	}
}

