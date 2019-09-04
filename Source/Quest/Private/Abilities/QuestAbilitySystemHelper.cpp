// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestAbilitySystemHelper.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "QuestGlobalTags.h"

void UQuestAbilitySystemHelper::GetTags(const AActor* Actor, FGameplayTagContainer& OutGameplayTags)
{
	OutGameplayTags = OutGameplayTags.EmptyContainer;

	if (!IsValid(Actor))
	{
		return;
	}

	const UAbilitySystemComponent* AbilitySystem = Actor->FindComponentByClass<UAbilitySystemComponent>();
	if (AbilitySystem == nullptr)
	{
		return;
	}

	AbilitySystem->GetOwnedGameplayTags(OutGameplayTags);
}

void UQuestAbilitySystemHelper::GetSourceAndTargetTags(const AActor* SourceActor, const AActor* TargetActor, FGameplayTagContainer& OutSourceTags, FGameplayTagContainer& OutTargetTags)
{
	GetTags(SourceActor, OutSourceTags);
	GetTags(TargetActor, OutTargetTags);
}

bool UQuestAbilitySystemHelper::DoesSatisfyTagRequirements(const FGameplayTagContainer& Tags, const FGameplayTagContainer& RequiredTags, const FGameplayTagContainer& BlockedTags)
{
	if (BlockedTags.Num() || RequiredTags.Num())
	{
		if (Tags.HasAny(BlockedTags))
		{
			return false;
		}
		if (!Tags.HasAll(RequiredTags))
		{
			return false;
		}
	}
	return true;
}

bool UQuestAbilitySystemHelper::DoesSatisfyTagRequirementsWithResult(const FGameplayTagContainer& Tags, const FGameplayTagContainer& InRequiredTags, const FGameplayTagContainer& InBlockedTags, FGameplayTagContainer& OutMissingTags, FGameplayTagContainer& OutBlockingTags)
{
	bool bSuccess = true;
	for (FGameplayTag Tag : InBlockedTags)
	{
		if (Tags.HasTag(Tag))
		{
			bSuccess = false;
			OutBlockingTags.AddTag(Tag);
		}
	}
	for (FGameplayTag Tag : InRequiredTags)
	{
		if (!Tags.HasTag(Tag))
		{
			bSuccess = false;
			OutMissingTags.AddTag(Tag);
		}
	}
	return bSuccess;
}

bool UQuestAbilitySystemHelper::IsVisibleForActor(const AActor* Actor, const AActor* Other)
{
	if (Actor == nullptr || Other == nullptr)
	{
		return false;
	}

	// TODO:  give each character a "visible component" that tells us what other characters can see that character
	// then use this function to see whether this character is one of the characters that can see the target;
	// If a character does not have a visible component, it is visible to everyone.

	return true;
}
