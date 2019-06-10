// Copyright 2019 Kelby Van Patten


#include "QuestRingItem.h"

UQuestRingItem::UQuestRingItem()
{
	ItemTypeEnum = EItemType::IT_Ring;
	bIsUsable = false;
	bCanBeStacked = true;
	bCanBeThrownAway = true;
	bCanBeCrafted = false;
	bCanBeEquipped = true;
	UseText = "Equip";
}
