// Copyright 2019 Kelby Van Patten


#include "QuestAmuletItem.h"

UQuestAmuletItem::UQuestAmuletItem()
{
	ItemTypeEnum = EItemType::IT_Amulet;
	bIsUsable = false;
	bCanBeStacked = true;
	bCanBeThrownAway = true;
	bCanBeCrafted = false;
	bCanBeEquipped = true;
	UseText = "Equip";
}