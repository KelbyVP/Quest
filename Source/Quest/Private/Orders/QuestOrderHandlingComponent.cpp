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
#include "QuestMerchantCharacter.h"
#include "QuestMeleeAttackOrder.h"
#include "QuestOrderHelperLibrary.h"
#include "QuestOrderCancellationPolicy.h"
#include "QuestOrder.h"
#include "QuestOrderData.h"
#include "QuestOrderTargetData.h"
#include "QuestPlayerController.h"
#include "QuestRangedAttackOrder.h"
#include "QuestStorage.h"
#include "QuestUseAbilityOrder.h"
#include "QuestWeaponItem.h"

// Sets default values for this component's properties
UQuestOrderHandlingComponent::UQuestOrderHandlingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bIsBeingDirectedByPlayer = false;
	bIsCurrentOrderComplete = false;

	// ...
}


// Called when the game starts
void UQuestOrderHandlingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UQuestOrderHandlingComponent::IssuePlayerDirectedOrderWithAbility(TSubclassOf<class UQuestGameplayAbility> Ability)
{
	AQuestCharacterBase* OrderedCharacter = Cast<AQuestCharacterBase>(GetOwner());
	if (!IsValid(OrderedCharacter))
	{
		return;
	}

	TSoftClassPtr<UQuestUseAbilityOrder> OrderType = Ability->GetDefaultObject<UQuestGameplayAbility>()->UseAbilityOrder;
	if (OrderType == nullptr)
	{
		return;
	}
	FQuestOrderData Order(OrderType, Ability);
	UE_LOG(LogTemp, Warning, TEXT
	("QuestOrderHandlingComponent::IssuePlayerDirectedOrderWithAbility: Casting ability %s"), *Ability->GetName());
	bIsBeingDirectedByPlayer = true;
	SetNextOrder(Order);
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
		bIsBeingDirectedByPlayer = true;
		TSoftClassPtr<UQuestOrder> OrderType;
		OrderedCharacter->AutoOrderComponent->GetWeaponAttackOrder(OrderType);
		FQuestOrderData Order(OrderType, TargetCharacter);
		OrderedCharacter->OrderHandlingComponent->SetNextOrder(Order);
		return;
	}

	/** If the target is a merchant, open his shop */
	AQuestMerchantCharacter* Merchant = Cast<AQuestMerchantCharacter>(TargetCharacter);
	if (IsValid(Merchant))
	{
		AQuestPlayerController* PlayerController = Cast<AQuestPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (IsValid(PlayerController))
		{
			AQuestCharacter* OwningCharacter = Cast<AQuestCharacter>(GetOwner());
			if (IsValid(OwningCharacter))
			{
				bIsBeingDirectedByPlayer = true;
				OwningCharacter->Merchant = Merchant;
				TSoftClassPtr<UQuestOrder> OrderType;
				OrderType = Merchant->OpenShopOrder;
				FQuestOrderData Order(OrderType, TargetCharacter);
				OrderedCharacter->OrderHandlingComponent->SetNextOrder(Order);
				return;
			}
		}
	}
}

void UQuestOrderHandlingComponent::IssuePlayerDirectedOrderWithTarget(FVector TargetLocation, TSoftClassPtr<UQuestOrder> MoveOrder)
{
	AQuestCharacterBase* OrderedCharacter = Cast<AQuestCharacterBase>(GetOwner());
	if (!IsValid(OrderedCharacter))
	{
		return;
	}
	bIsBeingDirectedByPlayer = true;
	FQuestOrderData Order(MoveOrder, TargetLocation);
	OrderedCharacter->OrderHandlingComponent->SetNextOrder(Order);
}

void UQuestOrderHandlingComponent::IssuePlayerDirectedOrderWithTarget(AQuestStorage* Storage, TSoftClassPtr<UQuestOrder> OpenStorageOrder)
{
	AQuestCharacterBase* OrderedCharacter = Cast<AQuestCharacterBase>(GetOwner());
	if (!IsValid(OrderedCharacter))
	{
		return;
	}
	bIsBeingDirectedByPlayer = true;
	AQuestCharacter* OwningCharacter = Cast<AQuestCharacter>(GetOwner());
	if (IsValid(OwningCharacter))
	{
		OwningCharacter->StorageChest = Storage;
		FQuestOrderData Order(OpenStorageOrder, Storage);
		OrderedCharacter->OrderHandlingComponent->SetNextOrder(Order);
	}
}

void UQuestOrderHandlingComponent::SetNextOrderAfterPlayerDirectedOrder()
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
	/** If we are in combat . . . */
	AQuestPlayerController* PlayerController = Cast<AQuestPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	TSoftClassPtr<UQuestOrder> AttackOrder = nullptr;

	/** If this character has an attack order available . . . */
	if (AutoOrderComponent->GetWeaponAttackOrder(AttackOrder))
	{
		/** if the character was attacking, try to attack the same character again, or else resume auto order mode */
		if (AttackOrder == CurrentOrderType)
		{
			if (OwningCharacter->TargetActor && !OwningCharacter->TargetActor->bIsDead)
			{
				SetNextOrder(FQuestOrderData(AttackOrder, OwningCharacter->TargetActor));
				return;
			}
			else
			{
				bIsBeingDirectedByPlayer = false;
				AutoOrderComponent->GenerateAutoOrder();
				return;
			}
		}

		AQuestCharacter* OwningQuestCharacter = Cast<AQuestCharacter>(OwningCharacter);

		/** If this character just finished moving successfully, attack if we can do so without moving */
		if (PlayerController && CurrentOrder.OrderType == PlayerController->MoveOrder)
		{
			AQuestAIController* Controller = Cast<AQuestAIController>(OwningCharacter->GetController());

			if (Controller && Controller->BehaviorTreeResult == EBTNodeResult::Succeeded)
			{
				TryToAttackWhileHolding();
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

		/** Otherwise, attack if we can do so without moving or else hold */
			TryToAttackWhileHolding();
			return;


	}

	/** Coming out of any other order, switch back to auto-order mode */
	bIsBeingDirectedByPlayer = false;
	AutoOrderComponent->GenerateAutoOrder();
}

void UQuestOrderHandlingComponent::TryToAttackWhileHolding()
{
	AQuestCharacterBase* OwningCharacter = Cast<AQuestCharacterBase>(GetOwner());
	TSoftClassPtr<UQuestOrder> AttackOrder = nullptr;
	if (OwningCharacter && OwningCharacter->AutoOrderComponent->GetWeaponAttackOrder(AttackOrder))
		// Find enemies in range
		{
			float Range = UQuestOrderHelperLibrary::GetRange(AttackOrder);
			TArray<AQuestCharacterBase*> HostileCharactersInAttackRange = UQuestOrderHelperLibrary::GetHostileTargetsInRange(OwningCharacter, Range);

			// If there are enemies in range, attack them
			if (HostileCharactersInAttackRange.Num() > 0)
			{
				OwningCharacter->AutoOrderComponent->GenerateAutoOrder();
				return;
			}
		}
	// If no enemies in range, issue hold order
	AQuestCharacter* OwningQuestCharacter = Cast<AQuestCharacter>(OwningCharacter);
	if (IsValid(OwningQuestCharacter))
	{
		if (OwningQuestCharacter->HoldOrder != nullptr)
		{
			SetNextOrder(FQuestOrderData(OwningQuestCharacter->HoldOrder));
			return;
		}
	}

	// If we don't have a hold order, issue default order
	SetNextOrder(FQuestOrderData(OwningCharacter->DefaultOrder));
	return;
}

void UQuestOrderHandlingComponent::SetNextOrder(const FQuestOrderData &NewOrder)
{
	//  Check whether this character has a cooldown tag
	FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag(FName(TEXT("cooldown")));
	if (Cast<AQuestCharacterBase>(GetOwner())->DoesCharacterHaveTag(CooldownTag)
		&& !UQuestOrderHelperLibrary::CanObeyWhileCooldownInEffect(GetOwner(), NewOrder.OrderType)
		)
	{
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
		if (bIsCurrentOrderComplete)
		{

			SetCurrentOrder(NewOrder);
			NextOrder.OrderType = nullptr;
			break;
		}
		else
		{
			NextOrder = NewOrder;
			break;
		}
	case EQuestOrderCancellationPolicy::INSTANT:
		IssueOrder(NewOrder);
	}
}

void UQuestOrderHandlingComponent::SetCurrentOrder(const FQuestOrderData &NewOrder)
{
	// TODO:  Cancel any existing current order
	CurrentOrder = NewOrder;
	bIsCurrentOrderComplete = false;
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
		ObeyOrder(Order);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestOrderHandlingComponent::Issue: could not obey order!"))
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
		Order.OrderType->GetDefaultObject<UQuestOrder>()->IssueOrder(Owner, TargetData, FQuestOrderCallback(), Order.Ability);
		break;
	case EQuestOrderCancellationPolicy::CAN_BE_CANCELLED:
	case EQuestOrderCancellationPolicy::CANNOT_BE_CANCELLED:
		{FQuestOrderCallback Callback;
		Callback.AddDynamic(this, &UQuestOrderHandlingComponent::OnOrderEndedCallback);
		if (!Order.OrderType.IsValid())
		{
			Order.OrderType.LoadSynchronous();
		}
		Order.OrderType->GetDefaultObject<UQuestOrder>()->IssueOrder(Owner, TargetData, Callback, Order.Ability);
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
		UE_LOG(LogTemp, Warning, TEXT("QuestOrderHandlingComponent::VerifyOrder: null pointer!"))
		return false;
	}

	/** verify the actor can issue this order */
	if (!UQuestOrderHelperLibrary::CanObeyOrder(OrderType.Get(), OrderedActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestOrderHandlingComponent::VerifyOrder: cannot obey!"))
		return false;
	}

	/** verify the target is valid */
	FQuestOrderTargetData TargetData = UQuestOrderHelperLibrary::CreateTargetDataForOrder(OrderedActor, Order.TargetActor, Order.TargetLocation);
	if (!UQuestOrderHelperLibrary::IsValidTarget(OrderType.Get(), OrderedActor, TargetData))
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestOrderHandlingComponent::VerifyOrder: no valid target!"))
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
	bIsCurrentOrderComplete = true;
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
			TODO:: Also need to take into account if we had a player directed ordedr; do we want to generate auto order, or maybe hold?
			(This gets called if, for example, the player tries to cast a spell but chooses an invalid target)
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
					SetNextOrderAfterPlayerDirectedOrder();
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

