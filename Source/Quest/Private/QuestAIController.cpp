// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestAIController.h"
#include "AIModule/Classes/Navigation/CrowdFollowingComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameplayTags/Classes/GameplayTagContainer.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "QuestAutoOrderComponent.h"
#include "QuestBlackboardHelperLibrary.h"
#include "QuestCharacter.h"
#include "QuestCharacterBase.h"
#include "QuestCharacterGroup.h"
#include "QuestOrder.h"
#include "QuestOrderHelperLibrary.h"
#include "QuestDefaultOrder.h"
#include "QuestCharacterBase.h"
#include "UObjectGlobals.h"

AQuestAIController::AQuestAIController(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
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
	if (Blackboard == nullptr)
	{
		return;
	}

	// TODO:  Do I want to do something when the BT is aborted?
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

void AQuestAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);


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

	/**	
	*	Subscribe to the AbilitySystemComponent's delegate that tells us when a cooldown tag has been added or removed
	*	so that when removed, we can call a new order
	*/
	FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag(FName(TEXT("cooldown")));
	ControlledPawn->AbilitySystemComponent->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AQuestAIController::OnCooldownTagChanged);
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

void AQuestAIController::IssueOrder(const FQuestOrderData& Order, FQuestOrderCallback Callback)
{

	UBehaviorTree* BehaviorTree = UQuestOrderHelperLibrary::GetBehaviorTree(Order.OrderType.Get());
	if (BehaviorTree == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestAIController::IssueOrder: Behavior tree is null pointer!"))
		Callback.Broadcast(EQuestOrderResult::FAILED);
		return;
	}

	CurrentOrderResultCallback = Callback;
	BehaviorTreeResult = EBTNodeResult::InProgress;
	SetBlackboardValues(Order);
	ApplyOrder(Order, BehaviorTree);
}


void AQuestAIController::BehaviorTreeEnded(EBTNodeResult::Type Result)
{
	//  TODO:  Instead of setting this variable and checking it in tick, why not just broadcast the delegates here?
	if (!VerifyBlackboard())
	{
		return;
	}

	switch (Result)
	{
	case EBTNodeResult::InProgress:
		return;
	case EBTNodeResult::Failed:
		BehaviorTreeResult = EBTNodeResult::Failed;
		return;
	case EBTNodeResult::Aborted:
		// TODO:  Do I want something to happen if the order is aborted?
		return;
	case EBTNodeResult::Succeeded:
		BehaviorTreeResult = EBTNodeResult::Succeeded;
		return;
	}
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
	if (Order.Ability != nullptr)
	{
		Blackboard->SetValueAsClass(UQuestBlackboardHelperLibrary::BLACKBOARD_KEY_ABILITY, Order.Ability.Get());
	}
}

void AQuestAIController::ApplyOrder(const FQuestOrderData& Order, UBehaviorTree* BehaviorTree)
{
		UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);

	if (BehaviorTreeComponent != nullptr && BehaviorTree != nullptr)
	{
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
			BehaviorTreeComponent->StartTree(*BehaviorTree, EBTExecutionMode::SingleRun);
		}
	}
}

void AQuestAIController::OnCooldownTagChanged(const FGameplayTag CooldownTag, int32 NewCount)
{
	CurrentOrderResultCallback.Broadcast(EQuestOrderResult::SUCCEEDED);
}

void AQuestAIController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	/** This part exists just in case the character is not a leader and doesn't have a character group; then it should become a leader and form one*/
	AQuestCharacterBase* ControlledCharacter = Cast<AQuestCharacterBase>(GetPawn());
	if (!ControlledCharacter->CharacterGroup)
	{
		ControlledCharacter->CreateCharacterGroup();
	}

	/** See whether our group should fight this character */
	if (IsValid(ControlledCharacter->CharacterGroup))
	{
		for (auto& PawnToCheck : DetectedPawns)
		{
			AQuestCharacterBase* CharacterDetected = Cast<AQuestCharacterBase>(PawnToCheck);
			if (CharacterDetected)
			{
				ControlledCharacter->CharacterGroup->CheckShouldStartFighting(CharacterDetected);
			}
		}
	}
}

