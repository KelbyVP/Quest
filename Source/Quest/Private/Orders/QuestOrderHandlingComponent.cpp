// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestOrderHandlingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "QuestAIController.h"
#include "QuestAttackOrder.h"
#include "QuestAutoOrderComponent.h"
#include "QuestCharacter.h"
#include "QuestCharacterBase.h"
#include "QuestCharacterGroup.h"
#include "QuestDefaultOrder.h"
#include "QuestHoldOrder.h"
#include "QuestOrderHelperLibrary.h"
#include "QuestOrderCancellationPolicy.h"
#include "QuestOrder.h"
#include "QuestOrderData.h"
#include "QuestOrderTargetData.h"
#include "QuestPlayerController.h"

// Sets default values for this component's properties
UQuestOrderHandlingComponent::UQuestOrderHandlingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bIsBeingDirectedByPlayer = false;

	// ...
}


// Called when the game starts
void UQuestOrderHandlingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UQuestOrderHandlingComponent::IssuePlayerDirectedOrderWithTarget(AQuestCharacterBase* TargetCharacter)
{
	AQuestCharacterBase* OrderedCharacter = Cast<AQuestCharacterBase>(GetOwner());
	if (!IsValid(OrderedCharacter))
	{
		return;
	}
	/** If the target is adverse, fight him */
	if (OrderedCharacter->IsAdverse(TargetCharacter))
	{
		// TODO: make sure the ordered character keeps following this order and doesn't get overriden by an auto-order if OnEnterCombat gets broadcast
		bIsBeingDirectedByPlayer = true;
		TSoftClassPtr<UQuestOrder> OrderType;
		OrderedCharacter->AutoOrderComponent->GetWeaponAttackOrder(OrderType);
		FQuestOrderData Order(OrderType, TargetCharacter);
		OrderedCharacter->OrderHandlingComponent->SetNextOrder(Order);
	}
}

void UQuestOrderHandlingComponent::IssuePlayerDirectedOrderWithTarget(FVector TargetLocation, TSoftClassPtr<UQuestOrder> MoveOrder)
{
	AQuestCharacterBase* OrderedCharacter = Cast<AQuestCharacterBase>(GetOwner());
	if (!IsValid(OrderedCharacter))
	{
		return;
	}
	// TODO: make sure the ordered character keeps following this order and doesn't get overriden by an auto-order if OnEnterCombat gets broadcast
	bIsBeingDirectedByPlayer = true;
	FQuestOrderData Order(MoveOrder, TargetLocation);
	OrderedCharacter->OrderHandlingComponent->SetNextOrder(Order);
}

void UQuestOrderHandlingComponent::IssuePlayerDirectedOrderWithTarget(AQuestStorage* Storage)
{

}

void UQuestOrderHandlingComponent::SetNextOrderBasedOnPlayerDirection()
{
	AQuestCharacterBase* OwningCharacter = Cast<AQuestCharacterBase>(GetOwner());

	if (!IsValid(OwningCharacter))
	{
		return;
	}

	auto AutoOrderComponent = OwningCharacter->AutoOrderComponent;
	if (!IsValid(AutoOrderComponent))
	{
		return;
	}
	/** If this character is not in combat, set to auto-order mode and use GenerateAutoOrder to get Default Order */
	if (!OwningCharacter->CharacterGroup->bIsInCombat)
	{
		bIsBeingDirectedByPlayer = false;
		AutoOrderComponent->GenerateAutoOrder();
		return;
	}
	
	TSoftClassPtr<UQuestOrder> CurrentOrderType = CurrentOrder.OrderType;
	if (!CurrentOrder.OrderType.IsValid())
	{
		CurrentOrder.OrderType.LoadSynchronous();
	}

	AQuestPlayerController* PlayerController = Cast<AQuestPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	TSoftClassPtr<UQuestOrder> AttackOrder = nullptr;
	/** If this character was  attacking . . . */
	if (AutoOrderComponent->GetWeaponAttackOrder(AttackOrder) && AttackOrder == CurrentOrderType)
	{
		/** If this character was attacking a target that's still alive, melee attack again */
		if (OwningCharacter->TargetActor && !OwningCharacter->TargetActor->bIsDead)
		{
			UE_LOG(LogTemp, Warning,
				TEXT("QuestOrderHandlingComponent::SetNextOrderBasedOnPlayerDirection: %s is doing a player-directed follow-on attack!"),
				*GetOwner()->GetName());
				SetNextOrder(FQuestOrderData(AttackOrder, OwningCharacter->TargetActor));
				return;
		}
		/** If target is lost or dead, resume auto order mode */
		else
		{
			bIsBeingDirectedByPlayer = false;
			AutoOrderComponent->GenerateAutoOrder();
			return;
		}
	}

	AQuestCharacter* OwningQuestCharacter = Cast<AQuestCharacter>(OwningCharacter);

	/** If this character was moving . . . */
	if (PlayerController && CurrentOrder.OrderType == PlayerController->MoveOrder)
	{
		// If the move succeeded, hold position
		AQuestAIController* Controller = Cast<AQuestAIController>(OwningCharacter->GetController());
		if (Controller && Controller->BehaviorTreeResult == EBTNodeResult::Succeeded)
		{
			SetNextOrder(FQuestOrderData(OwningQuestCharacter->HoldOrder));
			return;
		}
		// If move did not succeed, move to auto order mode
		else
		{
			bIsBeingDirectedByPlayer = false;
			AutoOrderComponent->GenerateAutoOrder();
			return;
		}
	}

	/** If this character had a hold order, hold position */
	if (CurrentOrder.OrderType == OwningQuestCharacter->HoldOrder)
	{
		SetNextOrder(FQuestOrderData(OwningQuestCharacter->HoldOrder));
		return;
	}

	/** Coming out of any other order, switch back to auto-order mode */
	bIsBeingDirectedByPlayer = false;
	AutoOrderComponent->GenerateAutoOrder();
}

void UQuestOrderHandlingComponent::SetNextOrder(const FQuestOrderData &NewOrder)
{
	//  Check whether this character has a cooldown tag
	FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag(FName(TEXT("cooldown")));
	if (Cast<AQuestCharacterBase>(GetOwner())->DoesCharacterHaveTag(CooldownTag)
		&& !UQuestOrderHelperLibrary::CanObeyWhileCooldownInEffect(GetOwner(), NewOrder.OrderType)
		)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestOrderHandlingComponent::SetNextOrder:  cooldown blocking trying to set next order %s"),
			*NewOrder.OrderType->GetName());
		NextOrder = NewOrder;
		return;
	}
	// TODO:  I'm suspicious that the CannotBeCancelled option will result in an order never getting called, because we already
	// got the callback from the current order, but we're preventing the new order from being called.  Need to debug and see. 
	//  Maybe it's as simple as checking the status of the BehaviorTreeResult in the AIController?
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
		// TODO:: Do we want it to do something if it can't be verified?
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
				// If this order should be determined based on a recent player order, issue appropriate order
				if (bIsBeingDirectedByPlayer)
				{
					SetNextOrderBasedOnPlayerDirection();
				}
				// If this order should not be determined based on a recent player order, generate auto order
				else
				{
					Character->AutoOrderComponent->GenerateAutoOrder();
				}
			}
		}
	}
}

