// Copyright 2019 Kelby Van Patten, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "QuestSpells.h"
#include "QuestGameplayAbility.generated.h"

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

class AQuestCharacterBase;

/**
 * 
 */
UCLASS()
class QUEST_API UQuestGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	
	UQuestGameplayAbility();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		ESpellType SpellType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		int SpellLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		FText Effects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		TArray<FGameplayTag> ResistanceTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestGameplayAbility)
		ESavingsThrowType SavingThrowAbilityType;

	UFUNCTION(BlueprintCallable, Category = QuestGameplayAbility)
		ESpellType GetSpellType();

	UFUNCTION(BlueprintCallable, Category = QuestGameplayAbility)
		int GetSpellLevel();
};
