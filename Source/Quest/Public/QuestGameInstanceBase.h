// Copyright 2019 Kelby Van Patten

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "QuestGameInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class QUEST_API UQuestGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()
	
public:

	/** Tracks the amount of gold the player has */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestGameInstanceBase")
	int Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestPlayerController")
		int MaxGold = 999999999;

	/** Increases the amount of gold the player has */
	UFUNCTION(BlueprintCallable, Category = "QuestGameInstanceBase")
		void IncreaseGold(int Amount);

	/** Decreases the amount of gold the player has */
	UFUNCTION(BlueprintCallable, Category = "QuestGameInstanceBase")
		void DecreaseGold(int Amount);

	/** A blueprint function that updates widgets that reflect the amount of gold the player has */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "UpdateGold"), Category = "QuestGameInstanceBase")
		void UpdateGold();
};
