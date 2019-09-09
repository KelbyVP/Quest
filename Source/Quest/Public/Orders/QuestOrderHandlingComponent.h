// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestOrderResult.h"
#include "QuestOrderHandlingComponent.generated.h"

struct FQuestOrderData;
class AQuestStorage;

/**
 * This component handles the orders for a character to execute.
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUEST_API UQuestOrderHandlingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestOrderHandlingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	void IssuePlayerDirectedOrderWithTarget(AQuestCharacterBase* TargetCharacter);
	void IssuePlayerDirectedOrderWithTarget(FVector TargetLocation, TSoftClassPtr<UQuestOrder> MoveOrder);
	void IssuePlayerDirectedOrderWithTarget(AQuestStorage* Storage);
	void SetNextOrder(const FQuestOrderData &NewOrder);
	void SetCurrentOrder(const FQuestOrderData &NewOrder);
	void IssueOrder(const FQuestOrderData& Order);
	void ObeyOrder(const FQuestOrderData& Order);
	bool TryCallNextOrder();
	bool VerifyOrder(const FQuestOrderData& Order) const;


private:

	FQuestOrderData NextOrder;
	FQuestOrderData CurrentOrder;

	/** Function called when FQuestOrderCallback delegate received */
	UFUNCTION()
	void OnOrderEndedCallback(EQuestOrderResult OrderResult);

	/** Begins next auto order when prior order is finished */
	void OrderEnded(EQuestOrderResult OrderResult);
};
