// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/QuestDamageExecutionCalculation.h"
#include "QuestFireballDamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class QUEST_API UQuestFireballDamageExecution : public UQuestDamageExecutionCalculation
{
	GENERATED_BODY()

	// Default constructor
	UQuestFireballDamageExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	UProperty* HealthProperty;
	FGameplayEffectAttributeCaptureDefinition HealthDef;
	
};
