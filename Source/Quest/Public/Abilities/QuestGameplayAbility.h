// Copyright 2019 Kelby Van Patten, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "QuestSpells.h"
#include "QuestGameplayAbility.generated.h"

class AQuestCharacterBase;

/**
 * 
 */
UCLASS()
class QUEST_API UQuestGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	
	UQuestGameplayAbility();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		ESpellType SpellType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		int SpellLevel = 1;

	UFUNCTION(BlueprintCallable, Category = QuestGameplayAbility)
		ESpellType GetSpellType();
};
