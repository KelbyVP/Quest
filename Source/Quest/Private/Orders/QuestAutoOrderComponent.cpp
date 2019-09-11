// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestAutoOrderComponent.h"
#include "QuestAttackOrder.h"
#include "QuestCharacter.h"
#include "QuestCharacterBase.h"
#include "QuestCharacterGroup.h"
#include "QuestDefaultOrder.h"
#include "QuestOrderData.h"
#include "QuestOrderHandlingComponent.h"
#include "QuestOrderHelperLibrary.h"
#include "QuestOrderTargetType.h"

// Sets default values for this component's properties
UQuestAutoOrderComponent::UQuestAutoOrderComponent()
{
		
}


// Called when the game starts
void UQuestAutoOrderComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UQuestAutoOrderComponent::GetWeaponAttackOrder(TSoftClassPtr<UQuestOrder>& InOrder)
{
	AQuestCharacterBase* OwningCharacter = Cast<AQuestCharacterBase>(GetOwner());
	if (OwningCharacter)
	{
		// If character has a weapon, set to the weapon's attack order
		if (OwningCharacter->EquippedWeapon != nullptr)
		{
			InOrder = OwningCharacter->EquippedWeapon->AttackOrder;
			return true;
		}
		
		// If no weapon, set to default order
		// TODO: Do we want to have some default hand-to-hand combat order?
		else
		{
			InOrder = OwningCharacter->DefaultOrder;
			return true;
		}
	}

	// Returns false if unable to set order
	return false;
}

bool UQuestAutoOrderComponent::IssueAutoOrder(const TSoftClassPtr<UQuestOrder> Order)
{

	AActor* Owner = GetOwner();
	if (!UQuestOrderHelperLibrary::CanObeyOrder(Order, Owner))
	{
		return false;
	}

	UQuestOrderHandlingComponent* OrderComponent = Owner->FindComponentByClass<UQuestOrderHandlingComponent>();
	if (OrderComponent == nullptr)
	{
		return false;
	}


	EQuestOrderTargetType TargetType = UQuestOrderHelperLibrary::GetTargetType(Order);
	switch (TargetType)
	{
		case EQuestOrderTargetType::NONE:
		{
			/** Orders with no target type are targeting self; may need to revise if we have a different situation */
			OrderComponent->SetNextOrder(FQuestOrderData(Order));
			break;
		}
		case EQuestOrderTargetType::ACTOR:
		{
			AQuestCharacterBase* TargetCharacter;
			TargetCharacter = UQuestOrderHelperLibrary::SelectTarget(Cast<AQuestCharacterBase>(Owner), Order);
			// TODO:  If there isn't a path to the selected actor, just pick the closest actor that there is a path to
			if (TargetCharacter)
			{
				OrderComponent->SetNextOrder(FQuestOrderData(Order, TargetCharacter));
			}
			break;
		}
		// TODO:  what do we want to do with other TargetTypes?
			return true;
	}

	return true;
}

void UQuestAutoOrderComponent::EnterCombat()
{
	bIsInCombat = true;

	AQuestCharacterBase* OwningCharacter = Cast<AQuestCharacterBase>(GetOwner());
	if (OwningCharacter && !OwningCharacter->OrderHandlingComponent->bIsBeingDirectedByPlayer)
	{
		GenerateAutoOrder();
	}
}

void UQuestAutoOrderComponent::GenerateAutoOrder()
{
	TSoftClassPtr<UQuestOrder> Order = nullptr;
	if (SelectAutoOrder(Order))
	{
		Order.LoadSynchronous();
		IssueAutoOrder(Order);
	}
}

bool UQuestAutoOrderComponent::SelectAutoOrder(TSoftClassPtr<UQuestOrder>& InOrder)
{
	AQuestCharacterBase* OwningCharacter = Cast<AQuestCharacterBase>(GetOwner());
	if (OwningCharacter)
	{
		if (!OwningCharacter->CharacterGroup->bIsInCombat)
		{
			InOrder = OwningCharacter->DefaultOrder;
			return (InOrder != nullptr);
		}
		/** If this is a QuestCharacter, use weapon attack */
		// TODO:  If a Quest Character is battling but not yet in the party, do we want them to use other auto orders?
		AQuestCharacter* OwningQuestCharacter = Cast<AQuestCharacter>(OwningCharacter);
		if (OwningQuestCharacter)
		{
			return (GetWeaponAttackOrder(InOrder));
		}

		/** If not a QuestCharacter, choose an order from AutoOrderArray */
		else if (!OwningQuestCharacter)
		{
			if (OwningCharacter->AutoOrderArray.Num() > 0)
			{
				for (auto& Order : OwningCharacter->AutoOrderArray)
				{
					Order.OrderType.LoadSynchronous();
					if (Order.bHasBeenUsed == false 
						 &&	UQuestOrderHelperLibrary::CanObeyOrder(Order.OrderType, OwningCharacter)
						)
					{
						InOrder = Order.OrderType;
						return true;
					}
				}
			}
			/** If unable to get order from array, issue melee order */
				return (GetWeaponAttackOrder(InOrder));
		}
	}
	return false;
}

