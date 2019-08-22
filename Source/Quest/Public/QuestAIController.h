// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "QuestOrder.h"
#include "QuestOrderData.h"
#include "QuestAIController.generated.h"

class AQuestGameMode;
class UQuestOrder;
/**
 *  Base AI class for controlling all character movement and orders
 */
UCLASS()
class QUEST_API AQuestAIController : public AAIController
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

	/** Tells us whether the character is in combat and should therefore follow combat orders */
	UPROPERTY(BlueprintReadWrite, Category = "QuestAIController", meta = (AllowPrivateAccess = true))
		bool bIsInCombat;

	/** Order that stops the character and sets at idle */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestAIController", meta = (AllowPrivateAccess = true))
		TSoftClassPtr<UQuestOrder> StopOrder;

	/** Blackboard to use for all data relevant to the character AI */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestAIController", meta = (AllowPrivateAccess = true))
		UBlackboardData* CharacterBlackboardAsset;

	/** Pointer to the current game mode */
	AQuestGameMode *GameMode;

	/** Caches result of behavior tree */
	EBTNodeResult::Type BehaviorTreeResult;

	/** A delegate from the QuestOrder class that gets called when the Behavior Tree is complete */
	FQuestOrderCallback CurrentOrderResultCallback;

	/** Checks whether we have a valid Blackboard */
	bool VerifyBlackboard() const;

	/** Set the value of blackboard keys to be used for the order we are about to execute */
	void SetBlackboardValues(const FQuestOrderData& Order, const FVector& HomeLocation);

	/** Begins execution of new order */
	void ApplyOrder(const FQuestOrderData& Order, UBehaviorTree* BehaviorTree);

	/** Called when combat mode begins or ends */
	void OnCombatEnd();

};
