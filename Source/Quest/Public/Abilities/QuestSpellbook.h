// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestSpells.h"
#include "QuestGameplayAbility.h"
#include "QuestSpellbook.generated.h"

class AQuestCharacterBase;

USTRUCT(BlueprintType)
struct FMemorizedSpellStruct
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MemorizedSpellStruct")
	TSubclassOf<class UQuestGameplayAbility> Spell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MemorizedSpellStruct)
	bool bCanBeCast = false;

	FMemorizedSpellStruct()
	{
	};
};

USTRUCT(BlueprintType)
struct FMemorizedSpellsArrayStruct
{
public:
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MemorizedSpellStruct")
		TArray<FMemorizedSpellStruct> Spells;

	FMemorizedSpellsArrayStruct()
	{
	};

	FMemorizedSpellStruct GetSpellStructAtIndex(int Index)
	{
		return Spells[Index];
	}
};

USTRUCT(BlueprintType)
struct FLearnedSpellsArrayStruct
{
public:
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LearnedSpellStruct")
		TArray < TSubclassOf<class UQuestGameplayAbility>> Spells;

	FLearnedSpellsArrayStruct()
	{
	};

	TSubclassOf<class UQuestGameplayAbility> GetSpellSAtIndex(int Index)
	{
		return Spells[Index];
	}
};

UCLASS()
class QUEST_API AQuestSpellbook : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestSpellbook();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = QuestSpellbook)
	AQuestCharacterBase *OwningCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestSpellbook)
	ESpellType SpellbookType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestSpellbook)
	TArray<FLearnedSpellsArrayStruct> LearnedSpells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestSpellbook)
	TArray<FMemorizedSpellsArrayStruct> MemorizedSpells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestSpellbook)
		TArray<int> NumberOfMemorizedSpellsByLevel;

	UFUNCTION(BlueprintCallable, Category = QuestSpellbook)
		void SetMemorizedArraySizes();

	UFUNCTION(BlueprintCallable, Category = QuestSpellbook)
		void SetSizeOfLearnedSpells();

	UFUNCTION(BlueprintCallable, Category = QuestSpellbook)
		bool FindEmptyMemorizedSpellSlotAtLevel(int Level, int& SlotIndex);

	UFUNCTION(BlueprintCallable, Category = QuestSpellbook)
		bool IsMemorizedSlotEmpty(int Level, int SlotIndex);

	// If there is a spell in the slot, returns an out reference to the spell struct in the slot
	UFUNCTION(BlueprintCallable, Category = QuestSpellbook)
		bool GetMemorizedSpellStructAtIndex(int Level, int SlotIndex, FMemorizedSpellStruct &SpellStruct);

	UFUNCTION(BlueprintCallable, Category = QuestSpellbook)
		bool IsCorrectSpellTypeForThisSpellbook(TSubclassOf<class UQuestGameplayAbility> SpellToCheck);

		int GetSpellLevel(TSubclassOf<class UQuestGameplayAbility> SpellToCheck);

	//  Adds the spell to the Spellbook as a learned spell
	UFUNCTION(BlueprintCallable, Category = QuestSpellbook)
		bool LearnSpell(TSubclassOf<class UQuestGameplayAbility> SpellToLearn);

	//  Adds the spell to the memorized spell slots if we have learned this spell
	UFUNCTION(BlueprintCallable, Category = QuestSpellbook)
		bool MemorizeSpell(TSubclassOf<class UQuestGameplayAbility> SpellToMemorize);

	//  Triggers blueprint function when spell memorized so blueprint can update widget
	UFUNCTION(BlueprintImplementableEvent, Category = QuestSpellbook)
		void BP_OnSpellMemorized(int Level, int SpellSlot);

	// Removes the spell from the memorized spell slots
	UFUNCTION(BlueprintCallable, Category = QuestSpellbook)
		bool RemoveMemorizedSpell(TSubclassOf<class UQuestGameplayAbility> SpellToRemove);

	// Checks whether this spell is already in the spellbook as a learned spell
	UFUNCTION(BlueprintCallable, Category = QuestSpellbook)
		bool IsSpellLearned(TSubclassOf<class UQuestGameplayAbility> Spell);

	// Enables casting on all memorized spells, primarily used when character rests
	UFUNCTION(BlueprintCallable, Category = QuestSpellbook)
		void EnableCastingOnMemorizedSpells();


};
