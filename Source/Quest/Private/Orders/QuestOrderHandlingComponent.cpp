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
	//  Check whether this character has a cooldown tag
	FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag(FName(TEXT("cooldown")));
	if (Cast<AQuestCharacterBase>(GetOwner())->DoesCharacterHaveTag(CooldownTag))
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestOrderHandlingComponent::SetNextOrder: character has cooldown tag!"));
		NextOrder = NewOrder;
		return;
	}

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

	if (VerifyOrder(Order))
	{
		/** TODO:  Tell it to execute the order */
		FString OrderName = Order.OrderType->GetName();
		ObeyOrder(Order);
	}
	else
	{
	}

	
}

void UQuestOrderHandlingComponent::ObeyOrder(const FQuestOrderData& Order)
{
	AActor* Owner = GetOwner();
	FQuestOrderTargetData TargetData = UQuestOrderHelperLibrary::CreateTargetDataForOrder(Owner, Order.TargetActor, Order.TargetLocation);

	/** Execute order, and register callback if order is not instant */
	switch (UQuestOrderHelperLibrary::GetCancellationPolicy(Order.OrderType))
	{
	case EQuestOrderCancellationPolicy::INSTANT:
		/** Do not register a callback if order is instant; current order continues executing and will still callback when finished */
		Order.OrderType->GetDefaultObject<UQuestOrder>()->IssueOrder(Owner, TargetData, FQuestOrderCallback());
		break;
	case EQuestOrderCancellationPolicy::CAN_BE_CANCELLED:
	case EQuestOrderCancellationPolicy::CANNOT_BE_CANCELLED:
		{FQuestOrderCallback Callback;
		Callback.AddDynamic(this, &UQuestOrderHandlingComponent::OnOrderEndedCallback);
		if (!Order.OrderType.IsValid())
		{
			Order.OrderType.LoadSynchronous();
		}
		Order.OrderType->GetDefaultObject<UQuestOrder>()->IssueOrder(Owner, TargetData, Callback);
		}
		break;
	default:
		check(0);
		break;
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
		return false;
	}

	/** verify the actor can issue this order */
	if (!UQuestOrderHelperLibrary::CanObeyOrder(OrderType.Get(), OrderedActor))
	{
		return false;
	}

	/** verify the target is valid */
	FQuestOrderTargetData TargetData = UQuestOrderHelperLibrary::CreateTargetDataForOrder(OrderedActor, Order.TargetActor, Order.TargetLocation);
	if (!UQuestOrderHelperLibrary::IsValidTarget(OrderType.Get(), OrderedActor, TargetData))
	{
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

