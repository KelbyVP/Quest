// Copyright 2019 Kelby Van Patten


#include "QuestShieldItem.h"

UQuestShieldItem::UQuestShieldItem()
{
	ItemTypeEnum = EItemType::IT_Shield;
	bIsUsable = false;
	bCanBeStacked = true;
	bCanBeThrownAway = true;
	bCanBeCrafted = false;
	bCanBeEquipped = true;
	SocketName = "shieldSocket";
	UseText = "Equip";
}