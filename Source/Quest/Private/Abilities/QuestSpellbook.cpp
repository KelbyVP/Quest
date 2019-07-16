// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestSpellbook.h"
#include "QuestSpells.h"
#include "Containers/Array.h"
#include "QuestGameplayAbility.h"

// Sets default values
AQuestSpellbook::AQuestSpellbook()
{
	//NumberOfMemorizedSpellsByLevel.SetNum(9);
}

// Called when the game starts or when spawned
void AQuestSpellbook::BeginPlay()
{
	Super::BeginPlay();
}

void AQuestSpellbook::SetMemorizedArraySizes()
{
	int NumberOfLevels = NumberOfMemorizedSpellsByLevel.Num();
	MemorizedSpells.SetNum(NumberOfLevels);
	for (int i = 0; i < NumberOfLevels; i++)
	{
		MemorizedSpells[i].Spells.SetNum(NumberOfMemorizedSpellsByLevel[i]);
	}
}

void AQuestSpellbook::SetSizeOfLearnedSpells()
{
	int HighestSpellLevel = 0;
	if (NumberOfMemorizedSpellsByLevel.Num() > 0)
		for (int i=0; i < NumberOfMemorizedSpellsByLevel.Num(); i++)
		{
			if (NumberOfMemorizedSpellsByLevel[i] > 0)
			{
				HighestSpellLevel = i + 1;
			}
			else break;
		}
	LearnedSpells.SetNum(HighestSpellLevel);
}

bool AQuestSpellbook::FindEmptyMemorizedSpellSlotAtLevel(int Level, int& SlotIndex)
{
	int Index = Level - 1;
	if (MemorizedSpells.IsValidIndex(Index))
	{
		TArray<FMemorizedSpellStruct> SpellsAtLevel = MemorizedSpells[Index].Spells;
		int Size = SpellsAtLevel.Num();
		for (int i = 0; i < Size; i++)
		{
			if (SpellsAtLevel.IsValidIndex(i))
			{
				if (!SpellsAtLevel[i].Spell)
				{
					SlotIndex = i;
					return true;
				}
			}
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

int AQuestSpellbook::GetSpellLevel(TSubclassOf<class UQuestGameplayAbility> SpellToCheck)
{
	if (SpellToCheck)
	{
		int SpellLevel = SpellToCheck->GetDefaultObject<UQuestGameplayAbility>()->GetSpellLevel();
		return SpellLevel;
	}
	return 0;
}

//  Adds the spell to the Spellbook as a learned spell
bool AQuestSpellbook::LearnSpell(TSubclassOf<class UQuestGameplayAbility> SpellToLearn)
{
	if (SpellToLearn && IsCorrectSpellTypeForThisSpellbook(SpellToLearn))
		{
			if (IsSpellLearned(SpellToLearn))
			{
				return false;
			}
			int SpellLevel = GetSpellLevel(SpellToLearn);
			int SpellLevelIndex = SpellLevel - 1;
			if (LearnedSpells.IsValidIndex(SpellLevelIndex))
			{
				LearnedSpells[SpellLevelIndex].Spells.AddUnique(SpellToLearn);
				return true;
			}
		}
	return false;
}

//  Adds the spell to the memorized spell slots if we have learned this spell
bool AQuestSpellbook::MemorizeSpell(TSubclassOf<class UQuestGameplayAbility> SpellToMemorize)
{
	int SpellLevel = GetSpellLevel(SpellToMemorize);
	int SpellLevelIndex = SpellLevel - 1;

	int EmptySlot = 0;
	if (FindEmptyMemorizedSpellSlotAtLevel(SpellLevel, EmptySlot))
	{
		if (SpellToMemorize && IsSpellLearned(SpellToMemorize))
		{
			FMemorizedSpellStruct SpellStruct = FMemorizedSpellStruct();
			SpellStruct.Spell = SpellToMemorize;
			if (MemorizedSpells.IsValidIndex(SpellLevelIndex))
			{
				MemorizedSpells[SpellLevelIndex].Spells[EmptySlot] = SpellStruct;
				return true;
			}
		}
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
	int SpellLevel = GetSpellLevel(SpellToRemove);
	int SpellLevelIndex = SpellLevel - 1;
	if (MemorizedSpells.IsValidIndex(SpellLevelIndex))
	{
		for (auto s : MemorizedSpells[SpellLevelIndex].Spells)
		{
			if (!s.bCanBeCast && s.Spell == SpellToRemove)
			{
				s = FMemorizedSpellStruct();
				return true;
			}
		}
		//  Otherwise, remove any available matching spell struct if one exists
		for (auto s : MemorizedSpells[SpellLevelIndex].Spells)
		{
			if (s.Spell == SpellToRemove)
			{
				s = FMemorizedSpellStruct();
				return true;
			}
		}
	}
	return false;
}

// Checks whether this spell is already in the spellbook as a learned spell
bool AQuestSpellbook::IsSpellLearned(TSubclassOf<class UQuestGameplayAbility> Spell)
{
	if (Spell)
	{
		if (!IsCorrectSpellTypeForThisSpellbook(Spell))
		{
			return false;
		}
		else
		{
			int SpellLevel = GetSpellLevel(Spell);
			int SpellLevelIndex = SpellLevel - 1;
			if (LearnedSpells.IsValidIndex(SpellLevelIndex) && LearnedSpells[SpellLevelIndex].Spells.Num() > 0)
			{
				for (const auto s : LearnedSpells[SpellLevelIndex].Spells)
				{
					if (s && s == Spell)
					{
						return true;
					}
				}
			}
			else
			{
				return false;
			}
		}
	}
	else 
	{
		return false;
	}
	return false;
}
