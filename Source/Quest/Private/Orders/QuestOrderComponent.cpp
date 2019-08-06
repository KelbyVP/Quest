// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestOrderComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "QuestAbilitySystemHelper.h"
#include "QuestAIController.h"
#include "QuestGlobalTags.h"
#include "QuestOrder.h"
#include "QuestOrderErrorTags.h"
#include "QuestOrderHelper.h"
#include "QuestOrderTargetData.h"
#include "QuestStopOrder.h"
#include "QuestSelectableComponent.h"

// Sets default values for this component's properties
UQuestOrderComponent::UQuestOrderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UQuestOrderComponent::BeginPlay()
{
	Super::BeginPlay();

	APawn* Pawn = Cast<APawn>(GetOwner());
	if (Pawn == nullptr)
	{
		return;
	}

	// Register to receive selection events so we know whether to show order previews
	SelectableComponent = Pawn->FindComponentByClass<UQuestSelectableComponent>();
	if (SelectableComponent != nullptr)
	{
		SelectableComponent->OnSelected.AddDynamic(this, &UQuestOrderComponent::OnSelected);
	}	

	// Reset current order
	CurrentOrder = FQuestOrderData();

	// Try to set the stop order
	AQuestAIController* Controller = Cast<AQuestAIController>(Pawn->GetController());
	if (Controller == nullptr)
	{
		return;
	}
	StopOrder = Controller->GetStopOrder();
	CurrentOrder = StopOrder;
	IssueOrder(StopOrder);
}

void UQuestOrderComponent::NotifyOnOrderChanged(const FQuestOrderData& NewOrder)
{
	UpdateOrderPreviews();
	OnOrderChanged.Broadcast(NewOrder);
}

void UQuestOrderComponent::IssueOrder(const FQuestOrderData& Order)
{
	AActor* Owner = GetOwner();

	/** verify that the character has authority to issue the order */
	if (!Owner->HasAuthority())
	{
		return;
	}

	/** Clear the order queue */
	OrderQueue.Empty();
	OnOrderQueueCleared.Broadcast();

	/** If we are already doing this exact order instance, do nothing */
	if (CurrentOrder == Order)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestOrderComponent::IssueOrder:  Already doing '%s'!"), *Order.ToString());
		return;
	}

	/** Make sure we have a valid order */
	if (!Order.OrderType.IsValid())
	{
		Order.OrderType.LoadSynchronous();
	}

	bIsHomeLocationSet = false;

	/** Abort the current order unless it is the StopOrder or current order is instant */
	if (CurrentOrder.OrderType != StopOrder &&
		UQuestOrderHelper::GetOrderProcessPolicy(Order.OrderType, Owner, Order.Index) != EQuestOrderProcessPolicy::INSTANT)
	{
		switch (UQuestOrderHelper::GetOrderProcessPolicy(CurrentOrder.OrderType, Owner, CurrentOrder.Index))
		{
		case EQuestOrderProcessPolicy::CAN_BE_CANCELLED:
			OrderCancelled();

			if (CheckOrder(Order))
			{
				ObeyOrder(Order);
			}
			else
			{
				ObeyStopOrder();
			}
			break;
		case EQuestOrderProcessPolicy::CANNOT_BE_CANCELLED:
			// Since we can't cancel current order, put this order in queue
			OrderQueue.Add(Order);
			break;
		case EQuestOrderProcessPolicy::INSTANT:
			// This should not happen, since instant orders should not be set as current orders.
			check(0);
			break;
		default:
			check(0);
			break;
		}
	}
	else
	{
		if (CheckOrder(Order))
		{
			ObeyOrder(Order);
		}
		else
		{
			ObeyStopOrder();
		}
	}
}

void UQuestOrderComponent::SetCurrentOrder(FQuestOrderData NewOrder)
{
	LastOrder = CurrentOrder;
	CurrentOrder = NewOrder;
	NotifyOnOrderChanged(CurrentOrder);
}

void UQuestOrderComponent::ObeyOrder(const FQuestOrderData& Order)
{
	AActor* Owner = GetOwner();
	FQuestOrderTargetData TargetData = UQuestOrderHelper::CreateOrderTargetData(Owner, Order.Target, Order.Location);

	// Find the order's home location.
	FVector HomeLocation;
	if (bIsHomeLocationSet)
	{
		HomeLocation = LastOrderHomeLocation;
		bIsHomeLocationSet = false;
	}

	else
	{
		HomeLocation = Owner->GetActorLocation();
	}

	switch (UQuestOrderHelper::GetOrderProcessPolicy(Order.OrderType, Owner, Order.Index))
	{
	case EQuestOrderProcessPolicy::INSTANT:
	{
		// Instant orders are executed directly without changing the current order.
		Order.OrderType->GetDefaultObject<UQuestOrder>()->IssueOrder(
			Owner,
			TargetData,
			Order.Index,
			FQuestOrderCallback(),
			HomeLocation
		); 
	}
	break;
	case EQuestOrderProcessPolicy::CAN_BE_CANCELLED:
	case EQuestOrderProcessPolicy::CANNOT_BE_CANCELLED:
	{
		if (CurrentOrder.OrderType != StopOrder)
		{
			UnregisterTagListeners(CurrentOrder);
		}
		SetCurrentOrder(Order);

		FQuestOrderCallback Callback;
		Callback.AddDynamic(this, &UQuestOrderComponent::OnOrderEndedCallback);
		UE_LOG(LogTemp, Warning, TEXT("QuestOrderComponent::ObeyOrder:  Subscribed!"))

		if (!Order.OrderType.IsValid())
		{
			Order.OrderType.LoadSynchronous();
		}
		if (Order.OrderType != StopOrder)
		{
			RegisterTagListeners(Order);
		}
		Order.OrderType->GetDefaultObject<UQuestOrder>()->IssueOrder(
			Owner,
			TargetData,
			Order.Index,
			Callback,
			HomeLocation
		);
	}
	break;
	default:
		check(0);
		break;
	}
}

bool UQuestOrderComponent::CheckOrder(const FQuestOrderData& Order) const
{
	FQuestOrderErrorTags OrderErrorTags;

	AActor* OrderedActor = GetOwner();
	if (!Order.OrderType.IsValid())
	{
		Order.OrderType.LoadSynchronous();
	}

	TSubclassOf<UQuestOrder> OrderType = Order.OrderType.Get();
	if (OrderType == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UQuestOrderComponent::CheckOrder: The order for actor %s is invalid."),
			*OrderedActor->GetName());
		return false;
	}

	if (!UQuestOrderHelper::CanObeyOrder(OrderType.Get(), OrderedActor, Order.Index, &OrderErrorTags))
	{
		LogOrderErrorMessage(
			FString::Printf(TEXT("UQuestOrderComponent::CheckOrder: The actor '%s' cannot obey the order '%s'."),
				*OrderedActor->GetName(), *OrderType->GetName()),
			OrderErrorTags);
		return false;
	}

	FQuestOrderTargetData TargetData = UQuestOrderHelper::CreateOrderTargetData(OrderedActor, Order.Target, Order.Location);
	if (!UQuestOrderHelper::IsValidTarget(OrderType.Get(), OrderedActor, TargetData, Order.Index, &OrderErrorTags))
	{
		LogOrderErrorMessage(
			FString::Printf(
				TEXT("UQuestOrderComponent::CheckOrder: The actor '%s' was told to execute the order '%s', but the "
					"target data is invalid: %s"),
				*OrderedActor->GetName(), *OrderType->GetName(), *TargetData.ToString()),
			OrderErrorTags);
		return false;
	}

	return true;
}

void UQuestOrderComponent::LogOrderErrorMessage(const FString& Message, const FQuestOrderErrorTags& OrderErrorTags) const
{
	FString FinalMessage = Message + TEXT(": ") + OrderErrorTags.ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalMessage);
}

void UQuestOrderComponent::OnOrderEndedCallback(EQuestOrderResult OrderResult)
{
	OrderEnded(OrderResult);
}

void UQuestOrderComponent::OrderEnded(EQuestOrderResult OrderResult)
{
	UE_LOG(LogTemp, Warning, TEXT("QuestOrderComponent::OrderEnded: The order component is calling a new stop order!"))
	AActor* Owner = GetOwner();

	switch (OrderResult)
	{
	case EQuestOrderResult::FAILED:
		if (StopOrder != nullptr)
		{
			// OrderCancelled called in IssueOrder
			IssueOrder(FQuestOrderData(StopOrder));
		}
		return;
	case EQuestOrderResult::CANCELLED:
		if (!UQuestOrderHelper::CanOrderBeConsideredAsSucceeded(
			CurrentOrder.OrderType, Owner,
			UQuestOrderHelper::CreateOrderTargetData(Owner, CurrentOrder.Target, CurrentOrder.Location),
			CurrentOrder.Index))
		{
			// Order cancelled in IssueOrder
			IssueOrder(FQuestOrderData(StopOrder));
			break;
		}
	case EQuestOrderResult::SUCCEEDED:
		if (OrderQueue.IsValidIndex(0))
		{
			const FQuestOrderData NewOrder = OrderQueue[0];
			if (CheckOrder(NewOrder))
			{
				OrderQueue.RemoveAt(0);
				ObeyOrder(NewOrder);
				return;
			}
			OrderQueue.Empty();
			ObeyStopOrder();
		}
		else if (StopOrder != nullptr)
		{
			ObeyStopOrder();
		}
		return;
	}
}

void UQuestOrderComponent::OrderCancelled()
{
	AActor* Owner = GetOwner();
	FQuestOrderTargetData TargetData = UQuestOrderHelper::CreateOrderTargetData(Owner, CurrentOrder.Target, CurrentOrder.Location);

	UClass* OrderType = nullptr;

	if (!CurrentOrder.OrderType.IsValid())
	{
		OrderType = CurrentOrder.OrderType.LoadSynchronous();
	}
	else
	{
		OrderType = CurrentOrder.OrderType.Get();
	}

	if (OrderType != nullptr)
	{
		OrderType->GetDefaultObject<UQuestOrder>()->OrderCancelled(Owner, TargetData, CurrentOrder.Index);
	}
}

void UQuestOrderComponent::RegisterTagListeners(const FQuestOrderData& Order)
{
	AActor* Owner = GetOwner();
	FQuestOrderTagRequirements TagRequirements;
	UQuestOrderHelper::GetOrderTagRequirements(Order.OrderType, Owner, Order.Index, TagRequirements);

	UAbilitySystemComponent* OwnerAbilitySystem = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);

	// Owner Tags

	if (OwnerAbilitySystem != nullptr)
	{
		FGameplayTagContainer OwnerTags;
		for (FGameplayTag Tag : TagRequirements.SourceRequiredTags)
		{
			OwnerTags.AddTagFast(Tag);
		}

		for (FGameplayTag Tag : TagRequirements.SourceBlockedTags)
		{
			OwnerTags.AddTag(Tag);
		}

		if (TagRequirements.TargetRequiredTags.HasTag(UQuestGlobalTags::Relationship_Visible()))
		{
			OwnerTags.AddTag(UQuestGlobalTags::Status_Detector());
		}

		// Register a callback for each of the tags to check if it was added or removed
		for (FGameplayTag Tag : OwnerTags)
		{
			FOnGameplayEffectTagCountChanged& Delegate = 
				OwnerAbilitySystem->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved);

			FDelegateHandle DelegateHandle = Delegate.AddUObject(this, &UQuestOrderComponent::OnOwnerTagsChanged);
			RegisteredOwnerTagEventHandles.Add(Tag, DelegateHandle);
		}
	}

	// Target tags

	EQuestOrderTargetType TargetType = UQuestOrderHelper::GetTargetType(Order.OrderType, Owner, Order.Index);
	if (TargetType == EQuestOrderTargetType::ACTOR)
	{
		UAbilitySystemComponent* TargetAbilitySystem =
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Order.Target);

		if (TargetAbilitySystem != nullptr)
		{
			FGameplayTagContainer TargetTags;
			for (FGameplayTag Tag : TagRequirements.TargetRequiredTags)
			{
				TargetTags.AddTagFast(Tag);
			}
			for (FGameplayTag Tag : TagRequirements.TargetBlockedTags)
			{
				TargetTags.AddTag(Tag);
			}
			if (TagRequirements.TargetRequiredTags.HasTag(UQuestGlobalTags::Relationship_Visible()))
			{
				TargetTags.AddTag(UQuestGlobalTags::Status_Stealthed());
			}

			for (FGameplayTag Tag : TargetTags)
			{
				FOnGameplayEffectTagCountChanged& Delegate =
					TargetAbilitySystem->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved);
				FDelegateHandle DelegateHandle = Delegate.AddUObject(this, &UQuestOrderComponent::OnTargetTagsChanged);
				RegisteredTargetTagEventHandles.Add(Tag, DelegateHandle);
			}
		}
	}
}

void UQuestOrderComponent::UnregisterTagListeners(const FQuestOrderData& Order)
{
	AActor* Owner = GetOwner();
	FQuestOrderTagRequirements TagRequirements;
	UQuestOrderHelper::GetOrderTagRequirements(Order.OrderType, Owner, Order.Index, TagRequirements);

	UAbilitySystemComponent* OwnerAbilitySystem = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);

	// Owner tags

	if (OwnerAbilitySystem != nullptr)
	{
		for (TPair<FGameplayTag, FDelegateHandle> Pair : RegisteredTargetTagEventHandles)
		{
			FOnGameplayEffectTagCountChanged& Delegate =
				OwnerAbilitySystem->RegisterGameplayTagEvent(Pair.Key, EGameplayTagEventType::NewOrRemoved);
			Delegate.Remove(Pair.Value);
		}
		RegisteredOwnerTagEventHandles.Empty();
	}

	// Target tags

	EQuestOrderTargetType TargetType = UQuestOrderHelper::GetTargetType(Order.OrderType, Owner, Order.Index);
	if (TargetType == EQuestOrderTargetType::ACTOR)
	{
		UAbilitySystemComponent* TargetAbilitySystem =
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Order.Target);

		if (TargetAbilitySystem != nullptr)
		{
			for (TPair<FGameplayTag, FDelegateHandle> Pair : RegisteredTargetTagEventHandles)
			{
				FOnGameplayEffectTagCountChanged& Delegate =
					TargetAbilitySystem->RegisterGameplayTagEvent(Pair.Key, EGameplayTagEventType::NewOrRemoved);

				Delegate.Remove(Pair.Value);
			}
			RegisteredTargetTagEventHandles.Empty();
		}
	}
}

void UQuestOrderComponent::OnTargetTagsChanged(const FGameplayTag Tag, int32 NewCount)
{
	FQuestOrderTagRequirements TagRequirements;
	UQuestOrderHelper::GetOrderTagRequirements(CurrentOrder.OrderType, GetOwner(), CurrentOrder.Index, TagRequirements);

	if ((NewCount && TagRequirements.TargetBlockedTags.HasTag(Tag)) ||
		(!NewCount && TagRequirements.TargetRequiredTags.HasTag(Tag)))
	{
		OrderEnded(EQuestOrderResult::CANCELLED);
	}

	else if (NewCount && Tag == UQuestGlobalTags::Status_Stealthed() &&
		TagRequirements.TargetRequiredTags.HasTag(UQuestGlobalTags::Relationship_Visible()))
	{
		if (!UQuestAbilitySystemHelper::IsVisibleForActor(GetOwner(), CurrentOrder.Target))
		{
			OrderEnded(EQuestOrderResult::CANCELLED);
		}
	}
}

void UQuestOrderComponent::OnOwnerTagsChanged(const FGameplayTag Tag, int32 NewCount)
{
	FQuestOrderTagRequirements TagRequirements;
	UQuestOrderHelper::GetOrderTagRequirements(CurrentOrder.OrderType, GetOwner(), CurrentOrder.Index, TagRequirements);

	if ((NewCount && TagRequirements.SourceBlockedTags.HasTag(Tag)) ||
		!NewCount && TagRequirements.SourceRequiredTags.HasTag(Tag))
	{
		OrderEnded(EQuestOrderResult::CANCELLED);
	}

	else if (!NewCount && Tag == UQuestGlobalTags::Status_Detector() && 
		TagRequirements.TargetRequiredTags.HasTag(UQuestGlobalTags::Relationship_Visible()))
	{ 
		if (!UQuestAbilitySystemHelper::IsVisibleForActor(GetOwner(), CurrentOrder.Target))
		{
			OrderEnded(EQuestOrderResult::CANCELLED);
		}
	}
}

void UQuestOrderComponent::ObeyStopOrder()
{
	if (StopOrder == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no stop order set."), *GetOwner()->GetName());
		return;
	}
	if (!StopOrder.IsValid())
	{
		StopOrder.LoadSynchronous();
	}
	ObeyOrder(FQuestOrderData(StopOrder));
}

void UQuestOrderComponent::OnSelected()
{
	UE_LOG(LogTemp, Warning, TEXT("Quest Order Component:  %s has been selected! No other instructions exist for this function."), *GetOwner()->GetName())
}

void UQuestOrderComponent::UpdateOrderPreviews()
{
	return;
}

