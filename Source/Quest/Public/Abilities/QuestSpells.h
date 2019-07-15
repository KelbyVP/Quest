// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestSpells.generated.h"

class UGameplayAbility;

/**
 * 
 */
UCLASS()
class QUEST_API UQuestSpells : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestSpells")
		TArray<TSubclassOf<class UGameplayAbility>> Spells;
	
};
