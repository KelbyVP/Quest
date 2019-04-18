// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "QuestAbilitySystemComponent.h"
#include "QuestAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 This holds all of the attributes used by abilities, and it instantiates a copy of this on every character
 */
UCLASS()
class QUEST_API UQuestAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	// constructor
	UQuestAttributeSet();

	// Current Health, when 0 we expect owner to die.  Capped by MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UQuestAttributeSet, Health)

	// MaxHealth is its own attribute, since it can be modified by GameplayEffects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UQuestAttributeSet, MaxHealth)
};
