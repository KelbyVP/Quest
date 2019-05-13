// Copyright 2019 Kelby Van Patten

#pragma once

#include "CoreMinimal.h"
#include "Abilities/QuestDamageExecutionCalculation.h"
#include "QuestHealthBoostExecCalc.generated.h"

/**
 * 
 */
UCLASS()
class QUEST_API UQuestHealthBoostExecCalc : public UQuestDamageExecutionCalculation
{
	GENERATED_BODY()

public:

	// default class constructor
	UQuestHealthBoostExecCalc();

protected:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	UProperty* HealthProperty;
	FGameplayEffectAttributeCaptureDefinition HealthDef;
	float CalculateHealthBoost() const;
};
