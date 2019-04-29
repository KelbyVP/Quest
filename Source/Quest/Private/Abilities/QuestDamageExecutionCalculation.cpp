// Copyright 2019 Kelby Van Patten


#include "QuestDamageExecutionCalculation.h"
#include "QuestAttributeSet.h"

//	
struct DamageStatics
{
	//	These macros create a Property and Def for each attribute (eg., StrengthProperty and StrengthDef)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorClass)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Agility)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Level)
	DamageStatics()
	{
		/**	These macros take (S, P, T, B) and set up the Def (eg., StrengthDef) for each property
		*	S = class
		*	P = name as defined in the macros above
		*	T = whether the attribute is one from the effect's source or target
		*	B = bool as to whether we're taking a snapshot of the calculation now or updating it (if it doesn't matter, set to true to save performance) */
		DEFINE_ATTRIBUTE_CAPTUREDEF(UQuestAttributeSet, ArmorClass, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UQuestAttributeSet, Agility, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UQuestAttributeSet, Strength, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UQuestAttributeSet, Level, Source, true);
	}
};

//	returns an instance of the DamageStatics struct
static DamageStatics& GetDamageStatics()
{
	static DamageStatics DamageStaticsVar;
	return DamageStaticsVar;
}

UQuestDamageExecutionCalculation::UQuestDamageExecutionCalculation()
{
	//	Note:  The following does the same thing for the Health attribute as the macros in the struct above do for AC, Agility, and Strength
	//	Sets HealthProperty pointer to the Health attribute
	HealthProperty = FindFieldChecked<UProperty>(UQuestAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UQuestAttributeSet, Health));
	
	/**	Sets variable to look at the property being affected, to look at either the source or the target for the effect, and the bool 
	being true means that it takes a snapshot rather than updating it over time */
	HealthDef = FGameplayEffectAttributeCaptureDefinition(HealthProperty, EGameplayEffectAttributeCaptureSource::Target, true);

	RelevantAttributesToCapture.Add(HealthDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ArmorClassDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().AgilityDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().StrengthDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().LevelDef);
}

void UQuestDamageExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//	Creates variables to contain the relevant attributes
	float TargetArmorClass = 0.0f;
	float SourceAgility = 0.0f;
	float SourceStrength = 0.0f;
	float SourceLevel = 0.0f;

	//	Sets the variables to a reference to the value of the relevant attributes
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ArmorClassDef, FAggregatorEvaluateParameters(), TargetArmorClass);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().AgilityDef, FAggregatorEvaluateParameters(), SourceAgility);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().StrengthDef, FAggregatorEvaluateParameters(), SourceStrength);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().LevelDef, FAggregatorEvaluateParameters(), SourceLevel);

	//	Determines whether the attack is successful and applies appropriate damage

	bool bIsAttackSuccessful = false;
	float FinalDamage = 0;
	int8 AttackTry = FMath::RandRange(1, 20);

	if (AttackTry == 20)
	{
		bIsAttackSuccessful = true;
	}
	else if (AttackTry == 1)
	{
		bIsAttackSuccessful = false;
	}
	else
	{
		bIsAttackSuccessful = TryAttack(SourceStrength, TargetArmorClass, SourceLevel, AttackTry);
	}

	//	Calculates the damage
	if (bIsAttackSuccessful == false)
	{
		FinalDamage = 0;
	}
	else
	{
		FinalDamage = CalculateMeleeDamage(SourceStrength, TargetArmorClass);
	}

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(HealthProperty, EGameplayModOp::Additive, -FinalDamage));
}

bool UQuestDamageExecutionCalculation::TryAttack(float Strength, float ArmorClass, float Level, int8 AttackTry) const
{
	// Modify the minimum successful attack number based on armor class and level
	int8 ACAdjustment = 0;
	float TargetArmorClassAdjustedByLevelOfAttacker = ArmorClass + Level + 1;
	if (TargetArmorClassAdjustedByLevelOfAttacker > 2)
	{
		ACAdjustment += (TargetArmorClassAdjustedByLevelOfAttacker - 2);
	}
	if (TargetArmorClassAdjustedByLevelOfAttacker < -3)
	{
		ACAdjustment += (TargetArmorClassAdjustedByLevelOfAttacker + 3);
	}

	//	Determine the strength modifier
	int8 StrengthAdjustment = (Strength / 2);
	StrengthAdjustment -= 5;

	// See whether attack succeeded
	int8 AttackSuccessThreshhold = 20 - ACAdjustment - StrengthAdjustment;
	if (AttackTry > AttackSuccessThreshhold)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float UQuestDamageExecutionCalculation::CalculateMeleeDamage(float Strength, float ArmorClass) const
{
	int8 StrengthAdjustment = (Strength / 2);
	StrengthAdjustment -= 5;
	return FMath::RandRange(1, 10) + StrengthAdjustment;
}
