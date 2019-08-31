// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "QuestOrderData.h"
#include "QuestAIController.generated.h"

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

	/** Issues the order specified */
	void IssueOrder(const FQuestOrderData& Order);


protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	/** Blackboard that holds data used by the AI */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestAIController", meta = (AllowPrivateAccess = true))
		UBlackboardData * CharacterBlackboard;

	bool VerifyBlackboard() const;
	void SetBlackboardValues(const FQuestOrderData& Order);
	void ApplyOrder(const FQuestOrderData& Order, UBehaviorTree* BehaviorTree);

};
