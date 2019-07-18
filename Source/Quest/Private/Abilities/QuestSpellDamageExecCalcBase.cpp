// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestSpellDamageExecCalcBase.h"
#include "GameplayTagsModule.h"
#include "QuestGameplayAbility.h"
#include "QuestCharacterBase.h"
#include "QuestAttributeSet.h"

struct SpellDamageStatics
{
	//	These macros create a Property and Def for each attribute (eg., StrengthProperty and StrengthDef)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Agility)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorClass)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Charm)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Constitution)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Intelligence)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Wisdom)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Level)

	SpellDamageStatics()
	{
		/**	These macros take (S, P, T, B) and set up the Def (eg., StrengthDef) for each property
		*	S = class
		*	P = name as defined in the macros above
		*	T = whether the attribute is one from the effect's source or target
		*	B = bool as to whether we're taking a snapshot of the calculation now or updating it (if it doesn't matter, set to true to save performance) */
		DEFINE_ATTRIBUTE_CAPTUREDEF(UQuestAttributeSet, Agility, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UQuestAttributeSet, ArmorClass, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UQuestAttributeSet, Charm, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UQuestAttributeSet, Constitution, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UQuestAttributeSet, Intelligence, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UQuestAttributeSet, Strength, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UQuestAttributeSet, Wisdom, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UQuestAttributeSet, Level, Source, true);
	}
};

//	returns an instance of the DamageStatics struct
static SpellDamageStatics& GetSpellDamageStatics()
{
	static SpellDamageStatics DamageStaticsVar;
	return DamageStaticsVar;
}

UQuestSpellDamageExecCalcBase::UQuestSpellDamageExecCalcBase()
{
	//	Note:  The following does the same thing for the Health attribute as the macros in the struct above do for AC, Agility, and Strength
//	Sets HealthProperty pointer to the Health attribute
	HealthProperty = FindFieldChecked<UProperty>(UQuestAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UQuestAttributeSet, Health));

	/**	Sets variable to look at the property being affected, to look at either the source or the target for the effect, and the bool
	being true means that it takes a snapshot rather than updating it over time */
	HealthDef = FGameplayEffectAttributeCaptureDefinition(HealthProperty, EGameplayEffectAttributeCaptureSource::Target, true);

	RelevantAttributesToCapture.Add(HealthDef);
	RelevantAttributesToCapture.Add(GetSpellDamageStatics().AgilityDef);
	RelevantAttributesToCapture.Add(GetSpellDamageStatics().ConstitutionDef);
	RelevantAttributesToCapture.Add(GetSpellDamageStatics().CharmDef);
	RelevantAttributesToCapture.Add(GetSpellDamageStatics().IntelligenceDef);
	RelevantAttributesToCapture.Add(GetSpellDamageStatics().StrengthDef);
	RelevantAttributesToCapture.Add(GetSpellDamageStatics().WisdomDef);
	RelevantAttributesToCapture.Add(GetSpellDamageStatics().LevelDef);
};

void UQuestSpellDamageExecCalcBase::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//	Creates variables to contain the relevant attributes
	float SourceLevel = 0.0f;
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const UGameplayAbility* AbilityRef = Spec.GetContext().GetAbility();
	const UQuestGameplayAbility* OwningAbility = Cast<UQuestGameplayAbility>(Spec.GetContext().GetAbility());
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->AvatarActor : nullptr;
	AQuestCharacterBase* TargetQuestCharacter = nullptr;
	if (TargetActor)
	{
		TargetQuestCharacter = Cast<AQuestCharacterBase>(TargetActor);
	}

	//	Sets the variables to a reference to the value of the relevant attributes
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetSpellDamageStatics().LevelDef, FAggregatorEvaluateParameters(), SourceLevel);

	// Calculate Damage
	int Damage = CalculateBaseDamageAmount(OwningAbility, SourceLevel);
	if (DoesMakeSavingThrow(OwningAbility, TargetQuestCharacter))
	{
		Damage = int(Damage / 2);
	}

	/** Cut damage in half for each relevant resistance tag that the target has */
	/** Start by getting the owning ability's array of resistance tags */

	TArray<FGameplayTag> RelevantResistanceTags;
	if (OwningAbility)
	{
		RelevantResistanceTags = OwningAbility->ResistanceTags;
	}

	/** Cut damage in half for each resistance tag that target character has */

	if (TargetQuestCharacter)
		{
			for (auto TagToCheck : RelevantResistanceTags)
			{
				if (TargetQuestCharacter->DoesCharacterHaveTag(TagToCheck))
				{
					Damage = Damage / 2;
				}
			}
		}

	// Output damage
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(HealthProperty, EGameplayModOp::Additive, -Damage));
}

bool UQuestSpellDamageExecCalcBase::DoesMakeSavingThrow(const UQuestGameplayAbility* OwningAbility, AQuestCharacterBase* TargetCharacter) const
{
	float TargetAbilityScore = 0.0f;
	bool bCanSave = true;

	switch (OwningAbility->SavingThrowAbilityType)
	{
	case ESavingsThrowType::IT_None:
		bCanSave = false;
		break;
	case ESavingsThrowType::IT_Agility:
		TargetAbilityScore = TargetCharacter->AttributeSetComponent->Agility.GetCurrentValue();
		break;
	case ESavingsThrowType::IT_Constitution:
		TargetAbilityScore = TargetCharacter->AttributeSetComponent->Constitution.GetCurrentValue();
		break;
	case ESavingsThrowType::IT_Charm:
		TargetAbilityScore = TargetCharacter->AttributeSetComponent->Charm.GetCurrentValue();
		break;
	case ESavingsThrowType::IT_Intelligence:
		TargetAbilityScore = TargetCharacter->AttributeSetComponent->Intelligence.GetCurrentValue();
		break;
	case ESavingsThrowType::IT_Strength:
		TargetAbilityScore = TargetCharacter->AttributeSetComponent->Strength.GetCurrentValue();
		break;
	case ESavingsThrowType::IT_Wisdom:
		TargetAbilityScore = TargetCharacter->AttributeSetComponent->Wisdom.GetCurrentValue();
		break;
	}

	//  Checks saving throw based on TargetAbilityScore
	if (bCanSave)
	{
		bool bMadeSavingThrow = false;
		int SavingThrow = rand() % 20 + 1;
		float AgilityModifier = (TargetAbilityScore - 10) / 2;
		float AbilityCheck = TargetCharacter->AttributeSetComponent->Level.GetCurrentValue() + (trunc(AgilityModifier));
		if (AbilityCheck >= SavingThrow)
		{
			return true;
		}
	}
	return false;
}

int UQuestSpellDamageExecCalcBase::CalculateBaseDamageAmount(const UQuestGameplayAbility* OwningAbility, float SourceCharacterLevel) const
{
	int Damage = 0;
	if (OwningAbility->BaseDamageDieSize >= 1)
	{
		for (int i = 1; i <= OwningAbility->BaseDamageNumberOfRolls; i++)
		{
			Damage = Damage + rand() % OwningAbility->BaseDamageDieSize + 1;
		}
	}
	if (SourceCharacterLevel >= OwningAbility->BonusDamageBeginningLevel && OwningAbility->BonusDamageDieSize >= 1)
	for (int i = OwningAbility->BonusDamageBeginningLevel; i <= SourceCharacterLevel; i++)
	{
		Damage = Damage + rand() % (OwningAbility->BonusDamageDieSize) + 1;
	}
	return Damage;
}
