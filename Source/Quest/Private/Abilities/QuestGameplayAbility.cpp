// Copyright 2019 Kelby Van Patten, All Rights Reserved.


#include "QuestGameplayAbility.h"
#include "QuestSpells.h"
#include "QuestCharacterBase.h"


UQuestGameplayAbility::UQuestGameplayAbility()
{
	SpellType = ESpellType::IT_Other;
	Range = 200.f;
	TargetType = EAbilityTargetType::IT_None;
}

ESpellType UQuestGameplayAbility::GetSpellType()
{
	return SpellType;
}

int UQuestGameplayAbility::GetSpellLevel()
{
	return SpellLevel;
}

void UQuestGameplayAbility::AdjustRangeByCharacterLevel(float CharacterLevel)
{
	Range += RangeIncreasePerLevel * (CharacterLevel - SpellLevel);
}
