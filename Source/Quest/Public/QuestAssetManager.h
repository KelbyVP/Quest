// Copyright 2019 Kelby Van Patten

#pragma once

#include "Quest.h"
#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "QuestAssetManager.generated.h"

class UQuestItem;
/**  Game implementation of asset manager, overrides functionality and stores game-specific types. */
UCLASS()
class QUEST_API UQuestAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	// Constructor and overrides
	UQuestAssetManager() {}
	virtual void StartInitialLoading() override;

	/** Static types for items */
	static const FPrimaryAssetType PotionItemType;
	static const FPrimaryAssetType SkillItemType;
	static const FPrimaryAssetType WeaponItemType;

	/** Returns the current AssetManager object */
	static UQuestAssetManager& Get();

	/**
 * Synchronously loads an RPGItem subclass, this can hitch but is useful when you cannot wait for an async load
 * This does not maintain a reference to the item so it will garbage collect if not loaded some other way
 *
 * @param PrimaryAssetId The asset identifier to load
 * @param bDisplayWarning If true, this will log a warning if the item failed to load
 */
	UQuestItem* ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning = true);

};
