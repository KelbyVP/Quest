// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "QuestOrder.h"
#include "QuestOrderData.h"
#include "QuestAIController.generated.h"

class UQuestOrder;
/**
 *  Base AI class for controlling all character movement and orders
 */
UCLASS()
class QUEST_API AQuestAIController : public AAIController
{
	GENERATED_BODY()

public:


	/** Default constructor */
	AQuestAIController();

	/** Called every tick */
	virtual void Tick(float DeltaTime) override;

	/** Checks whether the character has the specified type of order */
	UFUNCTION(BlueprintPure)
		bool HasOrder(TSubclassOf<UQuestOrder> OrderType) const;

	/** Gets a stop order */
	UFUNCTION(BlueprintPure, Category = QuestAIController)
		TSoftClassPtr<UQuestOrder> GetStopOrder() const;

	/** Called by the Behavior Tree when it has ended with the passed result */
	UFUNCTION(BlueprintCallable, Category = QuestAIController)
		void BehaviorTreeEnded(EBTNodeResult::Type Result);

	/** Tells the character to execute the order */
	void IssueOrder(const FQuestOrderData& Order, FQuestOrderCallback Callback, const FVector& HomeLocation);

protected:
	virtual void OnPossess(APawn* InPawn) override;


private:

	/** Order that stops the character and sets at idle */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestAIController", meta = (AllowPrivateAccess = true))
		TSoftClassPtr<UQuestOrder> StopOrder;

	/** Blackboard to use for all data relevant to the character AI */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestAIController", meta = (AllowPrivateAccess = true))
		UBlackboardData* CharacterBlackboardAsset;

	/** Checks whether we have a valid Blackboard */
	bool VerifyBlackboard() const;

	/** Caches result of behavior tree */
	EBTNodeResult::Type BehaviorTreeResult;

	/** A delegate from the QuestOrder class that gets called when the Behavior Tree is complete */
	FQuestOrderCallback CurrentOrderResultCallback;

	/** Set the value of blackboard keys to be used for the order we are about to execute */
	void SetBlackboardValues(const FQuestOrderData& Order, const FVector& HomeLocation);

	/** Begins execution of new order */
	void ApplyOrder(const FQuestOrderData& Order, UBehaviorTree* BehaviorTree);

};
