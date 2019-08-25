// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QuestAbilitySystemHelper.generated.h"

/**
 * 
 */
UCLASS()
class QUEST_API UQuestAbilitySystemHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/** Checks whether any required tags are missing or blocking tags are present */

	UFUNCTION(BlueprintPure, Category = "QuestAbilitySystemHelper")
		static bool DoesSatisfyTagRequirements(
			const FGameplayTagContainer& Tags,
			const FGameplayTagContainer& RequiredTags,
			const FGameplayTagContainer& BlockedTags);
	UFUNCTION(BlueprintPure, Category = "QuestAbilitySystemHelper")
		static bool DoesSatisfyTagRequirementsWithResult(
			const FGameplayTagContainer& Tags,
			const FGameplayTagContainer& InRequiredTags,
			const FGameplayTagContainer& InBlockedTags,
			FGameplayTagContainer& OutMissingTags,
			FGameplayTagContainer& OutBlockingTags);

	/** Whether the character can see the other actor */
	UFUNCTION(BlueprintPure, Category = "QuestAbilitySystemHelper")
		static bool IsVisibleForActor(const AActor* Actor, const AActor* Other);
};
