// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Orders/QuestOrder.h"
#include "QuestGameplayAbility.h"
#include "QuestUseAbilityOrder.generated.h"

/**
 * 
 */
UCLASS()
class QUEST_API UQuestUseAbilityOrder : public UQuestOrder
{
	GENERATED_BODY()

public:
	UQuestUseAbilityOrder();
	UQuestUseAbilityOrder(TSubclassOf<class UQuestGameplayAbility> InAbility);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestUseAbilityOrder")
		TSubclassOf<class UQuestGameplayAbility> Ability;
	
};
