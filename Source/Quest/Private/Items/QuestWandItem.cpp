// Copyright 2019 Kelby Van Patten


#include "QuestWandItem.h"

UQuestWandItem::UQuestWandItem()
{
	ItemTypeEnum = EItemType::IT_Wand;
	bIsUsable = true;
	bCanBeStacked = true;
	bCanBeThrownAway = true;
	bCanBeCrafted = false;
	bCanBeEquipped = true;
	UseText = "Equip";
}

