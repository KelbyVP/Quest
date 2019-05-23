// Copyright 2019 Kelby Van Patten


#include "QuestWeaponItem.h"

UQuestWeaponItem::UQuestWeaponItem()
{
	ItemTypeEnum = EItemType::IT_Weapon;
	bIsUsable = true;
	bCanBeStacked = true;
	bCanBeThrownAway = true;
	bCanBeCrafted = false;
	SocketName = "swordSocket";
	UseText = "Equip";
}