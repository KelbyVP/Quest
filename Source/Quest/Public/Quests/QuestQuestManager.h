// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoalData.h"
#include "QuestQuestBase.h"
#include "QuestQuestManager.generated.h"

class AQuestCharacter;
class AQuestPlayerController;

UCLASS()
class QUEST_API AQuestQuestManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestQuestManager();

	UFUNCTION(BlueprintCallable)
		bool AddNewQuest(TSubclassOf<AQuestQuestBase> QuestClass, bool ShouldStartImmediately);

	UFUNCTION(BlueprintCallable)
		void SelectNewCurrentQuest(AQuestQuestBase* Quest);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnAddNewQuest"))
		void BP_OnAddNewQuest(AQuestQuestBase* Quest);

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Quest Manager")
		AQuestCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Quest Manager")
		AQuestPlayerController* PlayerController;

	/** The quest that we have currently selected and are working on */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Quest Manager")
		AQuestQuestBase* CurrentQuest;

	/** All of the quests that we are currently working on */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Quest Manager")
		TArray<AQuestQuestBase*> CurrentQuests;

	/** All of the quests that we have completed */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Quest Manager")
		TArray<AQuestQuestBase*> CompletedQuests;

	/** All of the quests that we have failed */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Quest Manager")
		TArray<AQuestQuestBase*> FailedQuests;

	/** 
	*	All of our current, completed, and failed quests;
	*	used to ensure we do not get a new version of a quest we already have
	*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Quest Manager")
		TArray<TSubclassOf<AQuestQuestBase>> AllQuestClasses;

	/** The data for goal we are currently working on and have selected */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Quest Manager")
		FGoalData CurrentGoalData;
};
