// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestFireballDamageExecution.h"
#include "GameplayTagsModule.h"
#include "QuestCharacterBase.h"
#include "QuestAttributeSet.h"

struct FireballDamageStatics
{
	//	These macros create a Property and Def for each attribute (eg., StrengthProperty and StrengthDef)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Agility)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Level)

	FireballDamageStatics()
	{
		/**	These macros take (S, P, T, B) and set up the Def (eg., StrengthDef) for each property
		*	S = class
		*	P = name as defined in the macros above
		*	T = whether the attribute is one from the effect's source or target
		*	B = bool as to whether we're taking a snapshot of the calculation now or updating it (if it doesn't matter, set to true to save performance) */
		DEFINE_ATTRIBUTE_CAPTUREDEF(UQuestAttributeSet, Agility, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UQuestAttributeSet, Level, Source, true);
	}
};

//	returns an instance of the DamageStatics struct
static FireballDamageStatics& GetFireballDamageStatics()
{
	static FireballDamageStatics DamageStaticsVar;
	return DamageStaticsVar;
}

UQuestFireballDamageExecution::UQuestFireballDamageExecution()
{
	//	Note:  The following does the same thing for the Health attribute as the macros in the struct above do for AC, Agility, and Strength
//	Sets HealthProperty pointer to the Health attribute
	HealthProperty = FindFieldChecked<UProperty>(UQuestAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UQuestAttributeSet, Health));

	/**	Sets variable to look at the property being affected, to look at either the source or the target for the effect, and the bool
	being true means that it takes a snapshot rather than updating it over time */
	HealthDef = FGameplayEffectAttributeCaptureDefinition(HealthProperty, EGameplayEffectAttributeCaptureSource::Target, true);

	RelevantAttributesToCapture.Add(HealthDef);
	RelevantAttributesToCapture.Add(GetFireballDamageStatics().AgilityDef);
	RelevantAttributesToCapture.Add(GetFireballDamageStatics().LevelDef);
};

void UQuestFireballDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//	Creates variables to contain the relevant attributes
	float SourceAgility = 0.0f;
	float SourceLevel = 0.0f;

	//	Sets the variables to a reference to the value of the relevant attributes
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetFireballDamageStatics().AgilityDef, FAggregatorEvaluateParameters(), SourceAgility);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetFireballDamageStatics().LevelDef, FAggregatorEvaluateParameters(), SourceLevel);

	//  Checks saving throw based on agility
	bool bMadeSavingThrow = false;
	int SavingThrow = rand() % 20 + 1;
	float AgilityModifier = (SourceAgility - 10) / 2;
	float AbilityCheck = SourceLevel + (trunc(AgilityModifier));
	if (AbilityCheck >= SavingThrow)
	{
		bMadeSavingThrow = true;
	}

	// Calculate Damage
	int Damage = 0;
	int Level = int(SourceLevel);
	for (int a = 1; a <= Level; a = a + 1)
	{
		Damage = Damage + ((rand() % 6) + 1);
	}

	// Half damage if saving throw made
	if (bMadeSavingThrow)
	{
		Damage = int(Damage / 2);
	}

	// Cut damage in half if target has resistance tag
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->AvatarActor : nullptr;
	AQuestCharacterBase* TargetQuestCharacter = Cast<AQuestCharacterBase>(TargetActor);
	FName TagText = FName(TEXT("resistance.Fire"));
	FGameplayTag ResistanceTag = UGameplayTagsManager::Get().RequestGameplayTag(TagText);

	if (TargetQuestCharacter->DoesCharacterHaveTag(TagText))
	{
		Damage = Damage / 2;
	}

	// Output damage
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(HealthProperty, EGameplayModOp::Additive, -Damage));
}
