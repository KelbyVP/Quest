// Copyright 2019 Kelby Van Patten


#include "QuestScrollItem.h"

UQuestScrollItem::UQuestScrollItem()
{
	ItemTypeEnum = EItemType::IT_Scroll;
	bIsUsable = true;
	bCanBeStacked = true;
	bCanBeThrownAway = true;
	bCanBeCrafted = false;
	bCanBeEquipped = true;
	UseText = "Equip";
}

