// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestGlobalTags.h"
#include "AbilitySystemGlobals.h"
#include "GameplayTagContainer.h"

const FGameplayTag& UQuestGlobalTags::AbilityActivationFailure_NoTarget()
{
	static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("AbilityActivationFailure.NoTarget")));
	return Tag;
}

const FGameplayTag& UQuestGlobalTags::Status_Alive()
{
	static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("status.alive")));
	return Tag;
}

const FGameplayTag& UQuestGlobalTags::Status_Detector()
{
	static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("status.detector")));
	return Tag;
}

const FGameplayTag& UQuestGlobalTags::Status_Stealthed()
{
	static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("status.stealthed")));
	return Tag;
}

const FGameplayTag& UQuestGlobalTags::Relationship_Visible()
{
	static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("relationship.Visible")));
	return Tag;
}
