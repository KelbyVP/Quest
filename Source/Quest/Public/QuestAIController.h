// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "QuestAIController.generated.h"

struct FQuestOrderData;

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

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	/** Blackboard that holds data for this character */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestAIController", meta = (AllowPrivateAccess = true))
		UBlackboardData* CharacterBlackboard;

	void SetBlackboardValues(const FQuestOrderData& Order);

	/** Tells us whether there is a valid blackboard and outputs an error if there isn't */
	bool VerifyBlackboard() const;
	
};
