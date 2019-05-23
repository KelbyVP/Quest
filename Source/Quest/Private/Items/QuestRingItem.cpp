// Copyright 2019 Kelby Van Patten


#include "QuestRingItem.h"

UQuestRingItem::UQuestRingItem()
{
	ItemTypeEnum = EItemType::IT_Ring;
	bIsUsable = true;
	bCanBeStacked = true;
	bCanBeThrownAway = true;
	bCanBeCrafted = false;
	UseText = "Equip";
}
