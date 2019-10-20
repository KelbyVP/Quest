// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoalStatus.h"
#include "QuestData.h"
#include "QuestQuestBase.generated.h"

class AQuestQuestManager;
class UQuestItem;
struct FGoalData;
struct FQuestCompletedGoal;

UCLASS()
class QUEST_API AQuestQuestBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestQuestBase();

	/** Initializes variables when quest begins */
	UFUNCTION(BlueprintCallable)
		void SetUpStartingGoals();

	/** Updates our quest's goals */
	UFUNCTION(BlueprintCallable)
		void UpdateGoals();

	/** Adds the goal at the index of the GoalData's Goals array */
	UFUNCTION(BlueprintCallable)
		void AddGoal(int32 Index);

	UFUNCTION(BlueprintCallable)
		FGoalData GetGoalAtIndex(int32 Index);

	UFUNCTION(BlueprintCallable)
		bool IsItemAlreadyObtained(int32 GoalIndex);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "HasInventoryItem"))
		bool BP_HasInventoryItem(UQuestItem* Item);

	/** Blueprint function that handles the UI when a goal is completed */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnGoalCompleted"))
		void BP_OnGoalCompleted(int32 GoalIndex);

	UPROPERTY(BlueprintReadOnly, Category = "Quest Quest Base - do not modify in child classes")
		AQuestQuestManager* QuestManager;

	// The data that contains the details of the quest
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Quest Quest Base")
		FQuestQuestData QuestData;

	/** The goals that are available when the quest begins */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Quest Quest Base")
		TArray<int32> StartingGoalIndices;

	/** The goals that we are currently working on */
	UPROPERTY(BlueprintReadWrite, Category = "Quest Quest Base - do not modify in child classes")
		TArray<FGoalData> CurrentGoals;

	/** The goals that we have already completed for this quest */
	UPROPERTY(BlueprintReadWrite, Category = "Quest Quest Base - do not modify in child classes")
		TArray<FQuestCompletedGoal> CompletedGoals;

	/** The indices of the goals that we are currently working on for this quest */
	UPROPERTY(BlueprintReadWrite, Category = "Quest Quest Base - do not modify in child classes")
		TArray<int32> CurrentGoalIndices;

	/** The number of targets we have killed or items we have found for each of our current goals */
	UPROPERTY(BlueprintReadWrite, Category = "Quest Quest Base - do not modify in child classes")
		TArray<int32> AmountsOfTargetsObtained;

	/** The state of completion for this quest */
	UPROPERTY(BlueprintReadWrite, Category = "Quest Quest Base - do not modify in child classes")
		EGoalStatus Status;

	/** The current description for this quest */
	UPROPERTY(BlueprintReadWrite, Category = "Quest Quest Base - do not modify in child classes")
		FText CurrentDescription;




};
