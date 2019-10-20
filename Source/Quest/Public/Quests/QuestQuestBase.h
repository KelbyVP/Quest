// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoalStatus.h"
#include "QuestCompletedGoal.h"
#include "QuestData.h"
#include "QuestQuestBase.generated.h"

class AQuestQuestManager;
class APlayerController;
class AQuestCharacter;
class UQuestItem;
struct FGoalData;

UCLASS()
class QUEST_API AQuestQuestBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestQuestBase();

	/** I am implementing this tool in Blueprint because formatting FTexts in C++ is confusing 
	*	This function will be called once a goal is completed to add the updated description to the current description
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "UpdateCurrentDescription"))
		void BP_UpdateCurrentDescription(int32 GoalIndex);


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

	UFUNCTION(BlueprintCallable)
		bool CompleteGoal(int32 GoalIndex, bool IsGoalFailed);

	UFUNCTION(BlueprintCallable)
		void OnGoalEndsQuest(bool IsGoalFailed);

	UFUNCTION(BlueprintCallable)
		void GiveCompletionRewards(AQuestPlayerController* PlayerController, AQuestCharacter* PlayerCharacter);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "HasInventoryItem"))
		bool BP_HasInventoryItem(UQuestItem* Item);

	/** Blueprint function that handles the UI when a goal is completed */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnGoalCompleted"))
		void BP_OnGoalCompleted(int32 GoalIndex);

	/** Allows child classes to triggers some event if the goal succeeds */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnGoalSucceeded"))
		void BP_OnGoalSucceeded(int32 GoalIndex);

	/** Allows child classes to triggers some event if the goal fails */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnGoalFailed"))
		void BP_OnGoalFailed(int32 GoalIndex);

	/** Blueprint function that removes all widgets associated with this quest */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "RemoveWidgets"))
		void BP_RemoveWidgets();

	/** Blueprint function that updates the subgoals in the journal if this quest is selected */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "UpdateGoalsInJournal"))
		void BP_UpdateGoalsInJournal();


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
