// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestGlobalTags.h"
#include "GameplayTagContainer.h"

bool UQuestGlobalTags::CanTagBeStacked(const FGameplayTag& Tag)
{
	return
		Tag != FGameplayTag::RequestGameplayTag(FName(TEXT("relationship.Friendly"))) &&
		Tag != FGameplayTag::RequestGameplayTag(FName(TEXT("relationship.Hostile"))) &&
		Tag != FGameplayTag::RequestGameplayTag(FName(TEXT("relationship.Neutral"))) &&
		Tag != FGameplayTag::RequestGameplayTag(FName(TEXT("relationship.Self"))) &&
		Tag != FGameplayTag::RequestGameplayTag(FName(TEXT("relationship.Visible"))) &&
		Tag != FGameplayTag::RequestGameplayTag(FName(TEXT("status.alive")))
		;
}
