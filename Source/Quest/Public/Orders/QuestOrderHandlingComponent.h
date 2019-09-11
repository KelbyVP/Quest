// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestOrderResult.h"
#include "QuestOrderHandlingComponent.generated.h"

struct FQuestOrderData;
class AQuestCharacterBase;
class UQuestOrder;
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
	/** Tells us whether the player has given this character an order that affects subsequent orders;
*	If false, auto order component will generate the next order;
*	If true, next order depends on what the player's order was. */
	bool bIsBeingDirectedByPlayer;

	void IssuePlayerDirectedOrderWithTarget(AQuestCharacterBase* TargetCharacter);
	void IssuePlayerDirectedOrderWithTarget(FVector TargetLocation, TSoftClassPtr<UQuestOrder> MoveOrder);
	void IssuePlayerDirectedOrderWithTarget(AQuestStorage* Storage);
	void SetNextOrderBasedOnPlayerDirection();
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