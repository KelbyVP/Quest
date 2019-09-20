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
		for (int i = 0; i < NumberOfMemorizedSpellsByLevel.Num(); i++)
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

bool AQuestSpellbook::IsMemorizedSlotEmpty(int Level, int SlotIndex)
{
	if (MemorizedSpells.IsValidIndex(Level - 1))
	{
		FMemorizedSpellsArrayStruct SpellsStructAtLevel = MemorizedSpells[Level - 1];
		if (SpellsStructAtLevel.Spells.IsValidIndex(SlotIndex))
		{
			TSubclassOf<UQuestGameplayAbility> Slot = SpellsStructAtLevel.Spells[SlotIndex].Spell;
			if (Slot)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}

bool AQuestSpellbook::GetMemorizedSpellStructAtIndex(int Level, int SlotIndex, FMemorizedSpellStruct& SpellStruct)
{
	if (MemorizedSpells.IsValidIndex(Level - 1))
	{
		FMemorizedSpellsArrayStruct SpellsStructAtLevel = MemorizedSpells[Level - 1];
		if (SpellsStructAtLevel.Spells.IsValidIndex(SlotIndex))
		{
			TSubclassOf<UQuestGameplayAbility> SlottedAbility = SpellsStructAtLevel.Spells[SlotIndex].Spell;
			if (SlottedAbility)
			{
				SpellStruct = SpellsStructAtLevel.Spells[SlotIndex];
				return true;
			}
		}
	}
	SpellStruct = FMemorizedSpellStruct();
	return false;
}

bool AQuestSpellbook::GetLearnedSpellAtIndex(int Level, int SlotIndex, TSubclassOf<class UQuestGameplayAbility>& Spell)
{
	if (LearnedSpells.IsValidIndex(Level - 1))
	{
		FLearnedSpellsArrayStruct SpellsArrayStruct = LearnedSpells[Level - 1];
		if (SpellsArrayStruct.Spells.IsValidIndex(SlotIndex))
		{
			Spell = SpellsArrayStruct.GetSpellAtIndex(SlotIndex);
			if (Spell)
			{
				return true;
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
				BP_OnSpellMemorized(SpellLevel, EmptySlot);
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
		TArray<FMemorizedSpellStruct> SpellsAtLevel = MemorizedSpells[SpellLevelIndex].Spells;
		for (int i = 0; i < SpellsAtLevel.Num(); i++)
		{
			if (SpellsAtLevel.IsValidIndex(i))
			{
				if (SpellsAtLevel[i].bCanBeCast && SpellsAtLevel[i].Spell == SpellToRemove)
				{
					RemoveMemorizedSpellAtIndex(SpellToRemove, SpellLevel, i);
					return true;
				}
			}
		}
		//  Otherwise, remove any available matching spell struct if one exists
		for (int i = 0; i < SpellsAtLevel.Num(); i++)
		{
			if (SpellsAtLevel.IsValidIndex(i))
			{
				if (SpellsAtLevel[i].Spell == SpellToRemove)
				{
					RemoveMemorizedSpellAtIndex(SpellToRemove, SpellLevel, i);
					return true;
				}
			}
		}
	}
	return false;
}

bool AQuestSpellbook::RemoveMemorizedSpellAtIndex(TSubclassOf<class UQuestGameplayAbility> SpellToRemove, int Level, int Index)
{
	//  First check to see whether this is even the right type of spell
	if (!IsCorrectSpellTypeForThisSpellbook(SpellToRemove))
	{
		return false;
	}

	// Remove the spell if the level and index are valid and if they contain the spell we are trying to remove
	if (MemorizedSpells.IsValidIndex(Level - 1))
	{
		TArray<FMemorizedSpellStruct> &SpellsAtLevel = MemorizedSpells[Level - 1].Spells;
		if (SpellsAtLevel.IsValidIndex(Index))
		{
			if (SpellsAtLevel[Index].Spell == SpellToRemove)
			{
				SpellsAtLevel[Index] = FMemorizedSpellStruct();
				BP_OnSpellRemoved(SpellToRemove, Level, Index);
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

void AQuestSpellbook::EnableCastingOnMemorizedSpells()
{
	for (auto &SpellsAtLevelStruct : MemorizedSpells)
	{
		for (auto &SpellStruct : SpellsAtLevelStruct.Spells)
		{
			if (SpellStruct.Spell)
			{
				SpellStruct.bCanBeCast = true;
			}
		}
	}
}

void AQuestSpellbook::DisableCastingOnMemorizedSpell(TSubclassOf<class UQuestGameplayAbility> Spell)
{
	if (!IsValid(Spell))
	{
		return;
	}
	int SpellLevel = Spell->GetDefaultObject<UQuestGameplayAbility>()->GetSpellLevel();
	FMemorizedSpellsArrayStruct SpellsStruct = MemorizedSpells[SpellLevel - 1];
	for (int i = 0; i < SpellsStruct.Spells.Num(); i++)
	{
		TSubclassOf<class UQuestGameplayAbility> SpellAtIndex = SpellsStruct.Spells[i].Spell;
		if (SpellAtIndex && SpellAtIndex == Spell && SpellsStruct.Spells[i].bCanBeCast == true)
		{
			MemorizedSpells[SpellLevel - 1].Spells[i].bCanBeCast = false;
			BP_OnSpellCast(Spell, SpellLevel, i);
			return;
		}

	}
}
