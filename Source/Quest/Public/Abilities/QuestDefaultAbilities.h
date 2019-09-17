// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestDefaultAbilities.generated.h"

class UQuestGameplayAbility;

 /** Base class for all DefaultAbilities */
UCLASS(BlueprintType)
class QUEST_API UQuestDefaultAbilities : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DefaultAbilities)
		TArray<TSubclassOf<UQuestGameplayAbility>> DefaultAbilities;
};
