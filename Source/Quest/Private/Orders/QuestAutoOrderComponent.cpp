// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestAutoOrderComponent.h"
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
	AQuestCharacterBase* Owner = Cast<AQuestCharacterBase>(GetOwner());
	DefaultOrder = Owner->GetDefaultOrder();
	IssueAutoOrder(DefaultOrder);
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
	OrderComponent->SetCurrentOrder(FQuestOrderData(Order));
	return true;
}

