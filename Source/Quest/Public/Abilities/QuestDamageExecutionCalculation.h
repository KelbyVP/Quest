// Copyright 2019 Kelby Van Patten

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "QuestDamageExecutionCalculation.generated.h"

/**
 * 
 */
UCLASS()
class QUEST_API UQuestDamageExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
	//	Default constructor
	UQuestDamageExecutionCalculation();
	
	//	
	UProperty * HealthProperty;
	FGameplayEffectAttributeCaptureDefinition HealthDef;

public:

	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;

protected:

	bool TryAttack(float Strength, float ArmorClass, float Level, int8 AttackTry) const;
	float CalculateMeleeDamage(float Strength, float ArmorClass) const;
};
