// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "QuestCharacterBase.h"

UQuestAttributeSet::UQuestAttributeSet()
	: Health(30.0f)
	, MaxHealth(100.f)
	, Level (6.0f)
	, ArmorClass(6.f)
	, Strength (16.f)
	, Intelligence (11.f)
	, Wisdom (10.f)
	, Agility (12.f)
	, Constitution (10.f)
	, Charm (10.f)
{
}

void UQuestAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UQuestAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UQuestAttributeSet, Health))) {
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.0f, MaxHealth.GetCurrentValue()));
		Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.0f, MaxHealth.GetCurrentValue()));
		OnHealthChange.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());
		AQuestCharacterBase* CharacterOwner = Cast<AQuestCharacterBase>(GetOwningActor());
		if (Health.GetCurrentValue() == MaxHealth.GetCurrentValue()) {
			if (CharacterOwner) 
			{
				CharacterOwner->AddGameplayTag(CharacterOwner->FullHealthTag);
			}
		}
		else
			if (CharacterOwner) 
			{
				CharacterOwner->RemoveGameplayTag(CharacterOwner->FullHealthTag);
			}
	}
}

void UQuestAttributeSet::LevelUp()
{
	Level.SetCurrentValue(Level.GetCurrentValue() + 1.0f);
	Level.SetBaseValue(Level.GetBaseValue() + 1.0f);
}

