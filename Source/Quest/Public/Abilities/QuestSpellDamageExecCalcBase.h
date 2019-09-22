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

protected:

	// Default constructor
	UQuestSpellDamageExecCalcBase();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;

	UProperty* HealthProperty;
	FGameplayEffectAttributeCaptureDefinition HealthDef;
	//UQuestGameplayAbility* OwningAbility;

	virtual bool DoesMakeSavingThrow(const UQuestGameplayAbility* OwningAbility, AQuestCharacterBase* TargetCharacter) const;
	virtual int CalculateBaseDamageAmount(const UQuestGameplayAbility* OwningAbility, float SourceCharacterLevel) const;
};
