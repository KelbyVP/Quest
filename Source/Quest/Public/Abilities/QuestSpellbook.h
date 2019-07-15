// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestSpells.h"
#include "QuestGameplayAbility.h"
#include "QuestSpellbook.generated.h"

USTRUCT(BlueprintType)
struct FMemorizedSpellStruct
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LearnedSpellStruct")
	TSubclassOf<class UQuestGameplayAbility> Spell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LearnedSpellStruct)
	bool bCanBeCast = false;

	FMemorizedSpellStruct()
	{
	};
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestSpellbook)
	ESpellType SpellbookType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestSpellbook)
	TArray<TSubclassOf<class UQuestGameplayAbility>> LearnedSpells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestSpellbook)
	TArray<FMemorizedSpellStruct> MemorizedSpells;

	UFUNCTION(BlueprintCallable, Category = QuestSpellbook)
		bool FindEmptyMemorizedSpellSlot(int& SlotIndex);

	UFUNCTION(BlueprintCallable, Category = QuestSpellbook)
		bool IsCorrectSpellTypeForThisSpellbook(TSubclassOf<class UQuestGameplayAbility> SpellToCheck);

	//  Adds the spell to the Spellbook as a learned spell
	UFUNCTION(BlueprintCallable, Category = QuestSpellbook)
		bool LearnSpell(TSubclassOf<class UQuestGameplayAbility> SpellToLearn);

	//  Adds the spell to the memorized spell slots if we have learned this spell
	UFUNCTION(BlueprintCallable, Category = QuestSpellbook)
		bool MemorizeSpell(TSubclassOf<class UQuestGameplayAbility> SpellToMemorize);

	// Removes the spell from the memorized spell slots
	UFUNCTION(BlueprintCallable, Category = QuestSpellbook)
		bool RemoveMemorizedSpell(TSubclassOf<class UQuestGameplayAbility> SpellToRemove);

	// Checks whether this spell is already in the spellbook as a learned spell
	UFUNCTION(BlueprintCallable, Category = QuestSpellbook)
		bool IsSpellLearned(TSubclassOf<class UQuestGameplayAbility> Spell);

};
