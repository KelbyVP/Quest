// Copyright 2019 Kelby Van Patten


#include "QuestHealthBoostExecCalc.h"
#include "QuestAttributeSet.h"
#include "QuestGameplayAbility.h"
#include "GameplayTagsModule.h"

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
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const UGameplayAbility* AbilityRef = Spec.GetContext().GetAbility();
	const UQuestGameplayAbility* OwningAbility = Cast<UQuestGameplayAbility>(Spec.GetContext().GetAbility());
	float Level = Spec.GetLevel();

	float HealthBoost = CalculateHealthBoost(OwningAbility, Level);
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(HealthProperty, EGameplayModOp::Additive, HealthBoost));
}

float UQuestHealthBoostExecCalc::CalculateHealthBoost(const UQuestGameplayAbility* OwningAbility, float Level) const
{
	float MaxBonusLevel = OwningAbility->MaxHealingBonusLevel;
	FMath::Clamp(Level, 1.0f, MaxBonusLevel);

	float NumberOfDice = OwningAbility->NumberOfHealingDice;
	int DieSize = OwningAbility->HealingDieSize;

	float HealingAmount = 0.0f;
	for (int i = 0; i < NumberOfDice; i++)
	{
		HealingAmount += FMath::RandRange(1, DieSize);
	}
	HealingAmount += OwningAbility->HealingBonus;
	HealingAmount += OwningAbility->HealingBonusPerLevel * Level;
	return HealingAmount;
}
