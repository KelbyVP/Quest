// Copyright 2019 Kelby Van Patten


#include "QuestWeaponItem.h"

UQuestWeaponItem::UQuestWeaponItem()
{
	ItemTypeEnum = EItemType::IT_Weapon;
	bIsUsable = true;
	bCanBeStacked = true;
	bCanBeThrownAway = true;
	SocketName = "swordSocket";
	UseText = "Equip";
}