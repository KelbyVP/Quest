// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestOrderHandlingComponent.h"
#include "QuestAIController.h"
#include "QuestAutoOrderComponent.h"
#include "QuestCharacterBase.h"
#include "QuestDefaultOrder.h"
#include "QuestOrderHelperLibrary.h"
#include "QuestOrderCancellationPolicy.h"
#include "QuestOrder.h"
#include "QuestOrderData.h"
#include "QuestOrderTargetData.h"

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
	AQuestCharacterBase* Character = Cast<AQuestCharacterBase>(GetOwner());
	if (Character == nullptr)
	{
		return;
	}

	/** Clear next order */
	NextOrder.OrderType = nullptr;
	
	/** Load order if necessary */
	if (!Order.OrderType.IsValid())
	{
		Order.OrderType.LoadSynchronous();
	}

	/** Get our default order */
	TSoftClassPtr<UQuestOrder> DefaultOrder = Character->DefaultOrder;
	if (!DefaultOrder.IsValid())
	{
		DefaultOrder.LoadSynchronous();
	}

	if (VerifyOrder(Order))
	{
		/** TODO:  Tell it to execute the order */
		FString OrderName = Order.OrderType->GetName();
		UE_LOG(LogTemp, Warning, TEXT("QuestOrderHandlingComponent::IssueOrder: %s is going to execute the chosen order!"), *Character->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestOrderHandlingComponent::IssueOrder: %s is going to execute default order!"), *Character->GetName());
	}

	
}

bool UQuestOrderHandlingComponent::TryCallNextOrder()
{
	if (NextOrder.OrderType != nullptr)
	{
		FQuestOrderData NewCurrentorder = NextOrder;
		NextOrder.OrderType = nullptr;
		SetCurrentOrder(NewCurrentorder);
		return true;
	}
	else {return false;}
}

bool UQuestOrderHandlingComponent::VerifyOrder(const FQuestOrderData& Order) const
{
	AActor* OrderedActor = GetOwner();
	if (!Order.OrderType.IsValid())
	{
		Order.OrderType.LoadSynchronous();
	}

	/** verify the order is not a null pointer */
	TSubclassOf<UQuestOrder> OrderType = Order.OrderType.Get();
	if (OrderType == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestOrderHandlingComponent::VerifyOrder: null pointer as an order!"));
		return false;
	}

	/** verify the actor can issue this order */
	if (!UQuestOrderHelperLibrary::CanObeyOrder(OrderType.Get(), OrderedActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestOrderHandlingComponent::VerifyOrder: %s could not obey order!"), *OrderedActor->GetName());
		return false;
	}

	/** verify the target is valid */
	FQuestOrderTargetData TargetData = UQuestOrderHelperLibrary::CreateTargetDataForOrder(OrderedActor, Order.TargetActor, Order.TargetLocation);
	if (!UQuestOrderHelperLibrary::IsValidTarget(OrderType.Get(), OrderedActor, TargetData))
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestOrderHandlingComponent::VerifyOrder: %s could not get a valid target for the order!"), *OrderedActor->GetName());
		return false;
	}

	return true;
}

void UQuestOrderHandlingComponent::OnOrderEndedCallback(EQuestOrderResult OrderResult)
{
	OrderEnded(OrderResult);
}

void UQuestOrderHandlingComponent::OrderEnded(EQuestOrderResult OrderResult)
{
	AActor* Owner = GetOwner();
	AQuestCharacterBase* Character = Cast<AQuestCharacterBase>(Owner);
	TSoftClassPtr<UQuestDefaultOrder> DefaultOrder = Character->DefaultOrder;
	TSoftClassPtr<UQuestOrder> Order = nullptr;
	if (Character)
	{
		switch (OrderResult)
		{
		case EQuestOrderResult::FAILED:
			/**
			*	TODO: Upon failure of one order in the array, need to try next order and so on until one succeeds, and if none succeed, then attack,
			*	and then restart trying the array again
			*/
				if (DefaultOrder)
				{
					if (!TryCallNextOrder())
					{
						NextOrder.OrderType = nullptr;
						SetCurrentOrder(FQuestOrderData(DefaultOrder));
					};
				}
			return;
		case EQuestOrderResult::CANCELLED:
			// TODO: Even if order was cancelled, we may wish to treat it as successful (eg., cancelled after spell committed)
			// TODO: If not successful, do we want to do the same thing as suggested in the TODO for Failed, by cycling through orders?
				Character->AutoOrderComponent->GenerateAutoOrder();
				return;
		case EQuestOrderResult::SUCCEEDED:
			Character->SetAutoOrderAsUsed(CurrentOrder.OrderType);
			if (!TryCallNextOrder())
			{
				Character->AutoOrderComponent->GenerateAutoOrder();
			}
		}
	}
}

