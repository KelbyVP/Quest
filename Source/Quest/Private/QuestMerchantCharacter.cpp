// Copyright 2019 Kelby Van Patten


#include "QuestMerchantCharacter.h"
#include "QuestGlobalTags.h"

AQuestMerchantCharacter::AQuestMerchantCharacter()
{
	bIsHostile = false;
	Affiliation = ECharacterAffiliation::IT_Neutral;
	DefaultTags.AddUnique(UQuestGlobalTags::Relationship_Neutral());
	DefaultTags.AddUnique(UQuestGlobalTags::Status_Invulnerable());
}