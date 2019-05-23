// Copyright 2019 Kelby Van Patten


#include "QuestMaterialItem.h"

UQuestMaterialItem::UQuestMaterialItem()
{
	ItemTypeEnum = EItemType::IT_Material;
	bIsUsable = false;
	bCanBeStacked = true;
	bCanBeThrownAway = false;
	bCanBeCrafted = false;
	UseText = "Craft";
}
