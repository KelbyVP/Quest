// Copyright 2019 Kelby Van Patten


#include "QuestHealthBoostExecCalc.h"
#include "QuestAttributeSet.h"

UQuestHealthBoostExecCalc::UQuestHealthBoostExecCalc()
{
	//	Sets HealthProperty pointer to the Health attribute
	HealthProperty = FindFieldChecked<UProperty>(UQuestAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UQuestAttributeSet, Health));

	/**	Sets variable to look at the property being affected, to look at either the source or the target for the effect, and the bool
	being true means that it takes a snapshot rather than updating it over time */
	HealthDef = FGameplayEffectAttributeCaptureDefinition(HealthProperty, EGameplayEffectAttributeCaptureSource::Target, true);

	RelevantAttributesToCapture.Add(HealthDef);
}

void UQuestHealthBoostExecCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	float HealthBoost = CalculateHealthBoost();
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(HealthProperty, EGameplayModOp::Additive, HealthBoost));
}

float UQuestHealthBoostExecCalc::CalculateHealthBoost() const
{
	return (FMath::RandRange(1, 8));
}
