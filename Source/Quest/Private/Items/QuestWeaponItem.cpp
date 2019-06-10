// Copyright 2019 Kelby Van Patten


#include "QuestWeaponItem.h"

UQuestWeaponItem::UQuestWeaponItem()
{
	ItemTypeEnum = EItemType::IT_Weapon;
	bIsUsable = false;
	bCanBeStacked = true;
	bCanBeThrownAway = true;
	bCanBeCrafted = false;
	bCanBeEquipped = true;
	SocketName = "swordSocket";
	UseText = "Equip";
}