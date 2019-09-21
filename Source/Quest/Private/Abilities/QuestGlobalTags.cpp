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

const FGameplayTag& UQuestGlobalTags::Relationship_Hostile()
{
	static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("relationship.Hostile")));
	return Tag;
}

const FGameplayTag& UQuestGlobalTags::Relationship_Neutral()
{
	static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("relationship.Neutral")));
	return Tag;
}

const FGameplayTag& UQuestGlobalTags::Relationship_Self()
{
	static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("relationship.Neutral")));
	return Tag;
}

const FGameplayTag& UQuestGlobalTags::Relationship_Friendly()
{
	static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("relationship.Friendly")));
	return Tag;
}

const FGameplayTag& UQuestGlobalTags::Relationship_Visible()
{
	static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("relationship.Visible")));
	return Tag;
}

const FGameplayTag& UQuestGlobalTags::Status_Alive()
{
	static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("status.alive")));
	return Tag;
}

const FGameplayTag& UQuestGlobalTags::Status_Blessed()
{
	static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("status.blessed")));
	return Tag;
}

const FGameplayTag& UQuestGlobalTags::Cooldown()
{
	static FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("cooldown")));
	return Tag;
}
