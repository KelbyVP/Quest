// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/QuestDamageExecutionCalculation.h"
#include "QuestSpellDamageExecCalcBase.generated.h"

/**
 * 
 */

class UQuestGameplayAbility; 
class AQuestCharacterBase;

UCLASS()
class QUEST_API UQuestSpellDamageExecCalcBase : public UQuestDamageExecutionCalculation
{
	GENERATED_BODY()

		// Default constructor
		UQuestSpellDamageExecCalcBase();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;

	UProperty* HealthProperty;
	FGameplayEffectAttributeCaptureDefinition HealthDef;
	//UQuestGameplayAbility* OwningAbility;

	bool DoesMakeSavingThrow(const UQuestGameplayAbility* OwningAbility, AQuestCharacterBase* TargetCharacter) const;
	int CalculateBaseDamageAmount(const UQuestGameplayAbility* OwningAbility, float SourceCharacterLevel) const;
};
