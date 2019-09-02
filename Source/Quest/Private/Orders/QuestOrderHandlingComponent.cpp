// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestOrderHandlingComponent.h"
#include "QuestAIController.h"
#include "QuestOrderHelperLibrary.h"
#include "QuestOrderCancellationPolicy.h"
#include "QuestOrder.h"
#include "QuestOrderData.h"

// Sets default values for this component's properties
UQuestOrderHandlingComponent::UQuestOrderHandlingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UQuestOrderHandlingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UQuestOrderHandlingComponent::SetNextOrder(const FQuestOrderData &NewOrder)
{
	EQuestOrderCancellationPolicy CancellationPolicy = UQuestOrderHelperLibrary::GetCancellationPolicy(CurrentOrder.OrderType);
	switch (CancellationPolicy)
	{
	case EQuestOrderCancellationPolicy::CAN_BE_CANCELLED:
		SetCurrentOrder(NewOrder);
		NextOrder.OrderType = nullptr;
		break;
	case EQuestOrderCancellationPolicy::CANNOT_BE_CANCELLED:
		NextOrder = NewOrder;
		break;
	case EQuestOrderCancellationPolicy::INSTANT:
		IssueOrder(NewOrder);
	}
}

void UQuestOrderHandlingComponent::SetCurrentOrder(const FQuestOrderData &NewOrder)
{
	// TODO:  Cancel any existing current order
	CurrentOrder = NewOrder;
	// TODO:: create an order preview system so we see the range, etc. of the order, and update that system based on the target type, etc. of this order
	IssueOrder(CurrentOrder);
}

void UQuestOrderHandlingComponent::IssueOrder(const FQuestOrderData &Order)
{
	AQuestAIController* Controller = Cast<AQuestAIController>(Cast<APawn>(GetOwner())->GetController());
	if (Controller)
	{
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestOrderHandlingComponent::IssueOrder: controller NOT found for %s!"), *GetOwner()->GetName());
	}
}

