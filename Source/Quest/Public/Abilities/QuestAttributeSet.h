// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameFramework/Actor.h"
#include "QuestAbilitySystemComponent.h"
#include "QuestAttributeSet.generated.h"


/**
 This holds all of the attributes used by abilities, and it instantiates a copy of this on every character
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, float, Health, float, MaxHealth);

UCLASS()
class QUEST_API UQuestAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	// constructor
	UQuestAttributeSet();

	// Current Health, when 0 we expect owner to die.  Capped by MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestAttributeSet")
		FGameplayAttributeData Health;

	// MaxHealth is its own attribute, since it can be modified by GameplayEffects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestAttributeSet")
		FGameplayAttributeData MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestAttributeSet")
		FGameplayAttributeData Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestAttributeSet")
		FGameplayAttributeData ArmorClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestAttributeSet")
		FGameplayAttributeData Strength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestAttributeSet")
		FGameplayAttributeData Intelligence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestAttributeSet")
		FGameplayAttributeData Wisdom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestAttributeSet")
		FGameplayAttributeData Agility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestAttributeSet")
		FGameplayAttributeData Constitution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestAttributeSet")
		FGameplayAttributeData Charm;

	FOnHealthChange OnHealthChange;

	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UFUNCTION(BlueprintCallable, Category = "QuestAttributeSet")
		void LevelUp();


};
