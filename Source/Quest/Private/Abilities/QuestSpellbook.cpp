// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestSpellbook.h"
#include "QuestSpells.h"
#include "Containers/Array.h"
#include "QuestGameplayAbility.h"

// Sets default values
AQuestSpellbook::AQuestSpellbook()
{
}

// Called when the game starts or when spawned
void AQuestSpellbook::BeginPlay()
{
	Super::BeginPlay();
}
//  Adds the spell to the Spellbook as a learned spell
bool AQuestSpellbook::LearnSpell(TSubclassOf<class UQuestGameplayAbility> SpellToLearn)
{
	if (SpellToLearn)
	{
		if (IsSpellLearned(SpellToLearn))
		{
				return false;
		}
		LearnedSpells.AddUnique(SpellToLearn);
		return true;
	}
	return false;
}

//  Adds the spell to the memorized spell slots if we have learned this spell
bool AQuestSpellbook::MemorizeSpell(TSubclassOf<class UQuestGameplayAbility> SpellToMemorize)
{
	if (SpellToMemorize && IsSpellLearned(SpellToMemorize))
		{
			FMemorizedSpellStruct SpellStruct = FMemorizedSpellStruct();
			SpellStruct.Spell = SpellToMemorize;
			MemorizedSpells.Add(SpellStruct);
			return true;
		}
	return false;
}

// Removes the spell from the memorized spell slots
bool AQuestSpellbook::RemoveMemorizedSpell(TSubclassOf<class UQuestGameplayAbility> SpellToRemove)
{
	// First try to remove a spell struct that has already been cast
	for (auto s : MemorizedSpells)
	{
		if (!s.bCanBeCast && s.Spell == SpellToRemove)
			{
				s = FMemorizedSpellStruct();
				return true;
			}
	}
	//  Otherwise, remove any available matching spell struct
	for (auto s : MemorizedSpells)
	{
		if (s.Spell == SpellToRemove)
		{
			s = FMemorizedSpellStruct();
			return true;
		}
	}
	return false;
}

// Checks whether this spell is already in the spellbook as a learned spell
bool AQuestSpellbook::IsSpellLearned(TSubclassOf<class UQuestGameplayAbility> Spell)
{
	if (Spell)
	{
		for (const auto s : LearnedSpells)
		{
			if (s && s == Spell)
			{
				return true;
			}
		}
	}
	return false;
}
