// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestOrderResult.h"
#include "QuestOrderHandlingComponent.generated.h"

struct FQuestOrderData;

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
	void SetNextOrder(const FQuestOrderData &NewOrder);
	void SetCurrentOrder(const FQuestOrderData &NewOrder);
	void IssueOrder(const FQuestOrderData& Order);
	bool TryCallNextOrder();

private:

	FQuestOrderData NextOrder;
	FQuestOrderData CurrentOrder;

	/** Function called when FQuestOrderCallback delegate received */
	UFUNCTION()
	void OnOrderEndedCallback(EQuestOrderResult OrderResult);

	/** Begins next auto order when prior order is finished */
	void OrderEnded(EQuestOrderResult OrderResult);
};
