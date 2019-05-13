// Copyright 2019 Kelby Van Patten

#pragma once

#include "CoreMinimal.h"
#include "QuestDamageExecutionCalculation.h"
#include "QuestMeleeDamageExecCalc.generated.h"

/**
 * 
 */
UCLASS()
class QUEST_API UQuestMeleeDamageExecCalc : public UQuestDamageExecutionCalculation
{
	GENERATED_BODY()

		// Default constructor
	UQuestMeleeDamageExecCalc();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	UProperty* HealthProperty;
	FGameplayEffectAttributeCaptureDefinition HealthDef;

public:

protected:

	bool TryAttack(float Strength, float ArmorClass, float Level, int8 AttackTry) const;
	float CalculateMeleeDamage(float Strength, float ArmorClass) const;
};
