// Copyright 2019 Kelby Van Patten

#pragma once

#include "CoreMinimal.h"
#include "QuestItem.h"
#include "QuestPotionItem.generated.h"

/**  Native base class for potions, should be blueprinted */
UCLASS()
class QUEST_API UQuestPotionItem : public UQuestItem
{
	GENERATED_BODY()
	
public:
	// Constructor
	UQuestPotionItem()
	{
		ItemType = UQuestAssetManager::PotionItemType;
	}
};
