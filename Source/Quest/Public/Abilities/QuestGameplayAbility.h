// Copyright 2019 Kelby Van Patten, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "QuestSpells.h"
#include "QuestGameplayAbility.generated.h"

// Tells us what kind of savings throw will reduce or avoid the effects of this ability
UENUM(BlueprintType)
enum class ESavingsThrowType : uint8
{
	IT_None UMETA(DisplayName = "None"),
	IT_Agility UMETA(DisplayName = "Agility"),
	IT_Charm UMETA(DisplayName = "Charm"),
	IT_Constitution UMETA(DisplayName = "Constitution"),
	IT_Intelligence UMETA(DisplayName = "Intelligence"),
	IT_Strength UMETA(DisplayName = "Strength"),
	IT_Wisdom UMETA(DisplayName = "Wisdom")
};

// Tells us what type of target is valid for this ability
UENUM(BlueprintType)
enum class EAbilityTargetType : uint8
{
	// This Target Type is typically for abilities that target the character using the ability or a range around that character
	IT_None UMETA(DisplayName = "None"),
	
	// This Target Type is for abilities that target an adverse character or a range surrounding that character
	IT_HostileCharacter UMETA(DisplayName = "HostileCharacter"),

	// This Target Type is for abilities that target a friendly character or a range surrounding that character
	IT_AlliedCharacter UMETA(DisplayName = "AlliedCharacter"),

	// This Target Type is for abilities that target a location
	IT_Location UMETA(DisplayName = "Location"),
};

class AQuestCharacterBase;
class UQuestUseAbilityOrder;

/**
 * 
 */
UCLASS()
class QUEST_API UQuestGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	
	UQuestGameplayAbility();
	
	/** Name of spell effects used for UI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		FText Name;

	/** Whether the spell is used by a wizard, priest, paladin, or other */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		ESpellType SpellType;

	/** Level of spell within the spellbook */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		int SpellLevel = 1;

	/** Description of spell effects used for UI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		FText Effects;

	/** Description of spell used for UI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		FText Description;

	/** GameplayTags that reduce the effects of this spell */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		TArray<FGameplayTag> ResistanceTags;

	/** Type of saving throw to avoid or reduce effects of this spell */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		ESavingsThrowType SavingThrowAbilityType;

	/** How close the casting character must be to the target to cast this spell */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		float Range;

	/** The types of targets that this ability can be directed at; none usually means the character using the ability */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		EAbilityTargetType TargetType;

	/** Order that should be called to trigger this ability */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = QuestGameplayAbility)
		TSoftClassPtr<UQuestUseAbilityOrder> UseAbilityOrder;

	//* variables that determine the amount of damage done by a damaging ability */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		int BaseDamageNumberOfRolls = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		int BaseDamageDieSize = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		int BaseDamageModifierPerRoll = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		int BonusDamageBeginningLevel = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		int BonusDamageDieSize = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		int BonusDamageModifierPerRoll = 0;

	//* variables that determine the amount of healing done by a healing ability */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		int NumberOfHealingDice = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		int HealingDieSize = 6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		int HealingBonus = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		int HealingBonusPerLevel = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		int MaxHealingBonusLevel = 99;

	UFUNCTION(BlueprintCallable, Category = QuestGameplayAbility)
		ESpellType GetSpellType();

	UFUNCTION(BlueprintCallable, Category = QuestGameplayAbility)
		int GetSpellLevel();
};
