// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestBurningDamageExecCalc.h"
#include "GameplayTagsModule.h"
#include "QuestAttributeSet.h"
#include "QuestCharacterBase.h"
#include "QuestGameplayAbility.h"
#include "QuestGlobalTags.h"


void UQuestBurningDamageExecCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->AvatarActor : nullptr;
	AQuestCharacterBase* TargetQuestCharacter = nullptr;
	if (TargetActor)
	{
		TargetQuestCharacter = Cast<AQuestCharacterBase>(TargetActor);
	}
	if (!IsValid(TargetQuestCharacter))
	{
		return;
	}

	int Damage = FMath::RandRange(1, 4);

	// Halve damage if character has fire resistance tag
	if (TargetQuestCharacter->DoesCharacterHaveTag(UQuestGlobalTags::Resistance_Fire()))
	{
		Damage = Damage / 2;
	}

	// Output damage
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(HealthProperty, EGameplayModOp::Additive, -Damage));
}
