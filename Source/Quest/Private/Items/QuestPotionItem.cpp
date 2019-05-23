// Copyright 2019 Kelby Van Patten


#include "items/QuestPotionItem.h"

UQuestPotionItem::UQuestPotionItem()
{
	ItemTypeEnum = EItemType::IT_Potion;
	bIsUsable = true;
	bCanBeStacked = true;
	bCanBeThrownAway = true;
	bCanBeCrafted = false;
	UseText = "Drink";
}