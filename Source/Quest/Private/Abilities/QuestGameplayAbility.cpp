// Copyright 2019 Kelby Van Patten, All Rights Reserved.


#include "QuestGameplayAbility.h"
#include "QuestSpells.h"
#include "QuestCharacterBase.h"


UQuestGameplayAbility::UQuestGameplayAbility()
{
	SpellType = ESpellType::IT_Other;
}

ESpellType UQuestGameplayAbility::GetSpellType()
{
	return SpellType;
}
