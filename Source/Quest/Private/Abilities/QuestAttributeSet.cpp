// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "QuestCharacterBase.h"

UQuestAttributeSet::UQuestAttributeSet()
	: Health(100.0f)
	, MaxHealth(100.f)
	, Level (7.0f)
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

bool UQuestAttributeSet::DoesMakeSavingThrow(ESavingsThrowType SavingThrowType)
{
	float TargetAbilityScore = 0.0f;
	bool bCanSave = true;

	switch (SavingThrowType)
	{
	case ESavingsThrowType::IT_None:
		bCanSave = false;
		break;
	case ESavingsThrowType::IT_Agility:
		TargetAbilityScore = Agility.GetCurrentValue();
		break;
	case ESavingsThrowType::IT_Constitution:
		TargetAbilityScore = Constitution.GetCurrentValue();
		break;
	case ESavingsThrowType::IT_Charm:
		TargetAbilityScore = Charm.GetCurrentValue();
		break;
	case ESavingsThrowType::IT_Intelligence:
		TargetAbilityScore = Intelligence.GetCurrentValue();
		break;
	case ESavingsThrowType::IT_Strength:
		TargetAbilityScore = Strength.GetCurrentValue();
		break;
	case ESavingsThrowType::IT_Wisdom:
		TargetAbilityScore = Wisdom.GetCurrentValue();
		break;
	}

	//  Checks saving throw based on TargetAbilityScore
	if (bCanSave)
	{
		int SavingThrow = rand() % 20 + 1;
		float AgilityModifier = (TargetAbilityScore - 10) / 2;
		float AbilityCheck = Level.GetCurrentValue() + (trunc(AgilityModifier));
		if (AbilityCheck >= SavingThrow)
		{
			return true;
		}
	}
	return false;
}

