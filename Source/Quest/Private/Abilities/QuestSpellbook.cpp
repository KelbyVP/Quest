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

bool AQuestSpellbook::FindEmptyMemorizedSpellSlot(int &SlotIndex)
{
	int Size = MemorizedSpells.Num();
	for (int i = 0; i < Size; i++)
	{
		if (!MemorizedSpells[i].Spell)
		{
			SlotIndex = i;
			return true;
		}
	}
	return false;
}

bool AQuestSpellbook::IsCorrectSpellTypeForThisSpellbook(TSubclassOf<class UQuestGameplayAbility> SpellToCheck)
{
	if (SpellToCheck)
	{
		ESpellType SpellType = SpellToCheck->GetDefaultObject<UQuestGameplayAbility>()->GetSpellType();
		if (SpellType == SpellbookType)
		{
			return true;
		}
	}
	return false;
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
	int EmptySlot = 0;
	if (FindEmptyMemorizedSpellSlot(EmptySlot))
	{
		UE_LOG(LogTemp, Warning, TEXT("Empty slot found!"))
		if (SpellToMemorize && IsSpellLearned(SpellToMemorize))
		{
			FMemorizedSpellStruct SpellStruct = FMemorizedSpellStruct();
			SpellStruct.Spell = SpellToMemorize;
			MemorizedSpells[EmptySlot] = SpellStruct;
			return true;
		}
	}
	if (!FindEmptyMemorizedSpellSlot(EmptySlot))
	{
		UE_LOG(LogTemp, Warning, TEXT("No Empty slot found!"))
	}
	return false;
}

// Removes the spell from the memorized spell slots
bool AQuestSpellbook::RemoveMemorizedSpell(TSubclassOf<class UQuestGameplayAbility> SpellToRemove)
{
	//  First check to see whether this is even the right type of spell
	if(!IsCorrectSpellTypeForThisSpellbook(SpellToRemove))
	{
		return false;
	}

	// If it is the right type of spell, try to remove a spell struct that has already been cast
	for (auto s : MemorizedSpells)
	{
		if (!s.bCanBeCast && s.Spell == SpellToRemove)
			{
				s = FMemorizedSpellStruct();
				return true;
			}
	}
	//  Otherwise, remove any available matching spell struct if one exists
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
