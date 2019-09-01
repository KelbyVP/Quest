// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "QuestOrderData.h"
#include "QuestAIController.generated.h"

class UAISenseConfig_Sight;
class AQuestCharacterBase;

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

	UFUNCTION()
		void InitializePerceptionComponent(AQuestCharacterBase* ControlledPawn);






private:
	/** Blackboard that holds data used by the AI */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QuestAIController", meta = (AllowPrivateAccess = true))
		UBlackboardData * CharacterBlackboard;

	/** Variables for the AI Perception System */
	UPROPERTY()
		UAISenseConfig_Sight* SightConfig;




	bool VerifyBlackboard() const;
	void SetBlackboardValues(const FQuestOrderData& Order);
	void ApplyOrder(const FQuestOrderData& Order, UBehaviorTree* BehaviorTree);

	/** Function called when perception component sees something */
	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*>& DetectedPawns);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "QuestAIController")
		float AISightRadius = 500.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "QuestAIController")
		float AISightAge = 5.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "QuestAIController")
		float AILoseSightRadius = AISightRadius + 50.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "QuestAIController")
		float AIFieldOfView = 90.0f;

	/** Issues the order specified */
	void IssueOrder(const FQuestOrderData& Order);


};
