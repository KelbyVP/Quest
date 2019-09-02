// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestAutoOrderComponent.h"
#include "QuestAttackOrder.h"
#include "QuestCharacter.h"
#include "QuestCharacterBase.h"
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
	//switch (TargetType)
	//{
	//	case EQuestOrderTargetType::NONE:
	//	{
	//		OrderComponent->SetNextOrder(FQuestOrderData(Order));
	//	}


	//	// TODO:: plenty more code goes here to sort out what the order looks like and set it as the next order in the order handling component
	//   // Order-Abilities gets the target type here and tells the OrderComponent to issue the order through the Order Component's InsertOrderBeforeCurrentOrder function




	//	FString Name;
	//	if (Order) { Name = Order->GetName(); }
	//	else { Name = FString(TEXT("None")); }

	//	UE_LOG(LogTemp, Warning, TEXT("QuestAutoOrderComponent::IssueAutoOrder: calling order %s"), *Name)
	//		return true;
	//}
	OrderComponent->SetNextOrder(FQuestOrderData(Order));
	return true;
}

void UQuestAutoOrderComponent::EnterCombat()
{
	bIsInCombat = true;
	TSoftClassPtr<UQuestOrder> Order = nullptr;
	if (SelectAutoOrder(Order))
	{
		FString CharacterName = GetOwner()->GetName();
		FString OrderName = Order->GetName();
		UE_LOG(LogTemp, Warning, TEXT("QuestAutoOrderComponent::EnterCombat:  %s selected order %s!"), *CharacterName, *OrderName);
		IssueAutoOrder(Order);
	}
}

bool UQuestAutoOrderComponent::SelectAutoOrder(TSoftClassPtr<UQuestOrder>& InOrder)
{
	AQuestCharacterBase* OwningCharacter = Cast<AQuestCharacterBase>(GetOwner());
	if (OwningCharacter)
	{
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
					if (Order.bHasBeenUsed == false &&
						UQuestOrderHelperLibrary::CanObeyOrder(Order.OrderType, OwningCharacter))
					{
						InOrder = Order.OrderType;
						break;
					}
				}
			}
			/** If unable to get order from array, issue melee order */
			else return (GetWeaponAttackOrder(InOrder));
		}
		else return false;
	}
	return false;
}

