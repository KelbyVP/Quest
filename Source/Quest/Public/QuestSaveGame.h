// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Quest.h"
#include "QuestItem.h"
#include "GameFramework/SaveGame.h"
#include "QuestSaveGame.generated.h"

/**  List of versions, native code will handle fixups for any old versions */
namespace EQuestSaveGameVersion
{
	enum type
	{
		// Initial version
		Initial,
		// Added inventory
		AddedInventory,
		// Added ItemData to store count/level
		AddedItemData,

		// ----------------<new versions must be added before this line>-----------------------------
		VersionPlusOne,
		LatestVersion = VersionPlusOne - 1
	};
}

/** Object that is written to and read from the save game archive, with a data version */
UCLASS(BlueprintType)
class QUEST_API UQuestSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/** constructor */
	UQuestSaveGame()
	{
		// Set to current version, this will get overwritten during serialization when loading
		SavedDataVersion = EQuestSaveGameVersion::LatestVersion;
	}

	/** Map of items to item data */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
		TMap<FPrimaryAssetId, FQuestItemData> InventoryData;

	/** Map of slotted items */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
		TMap<FQuestItemSlot, FPrimaryAssetId> SlottedItems;

	/** User's unique id */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
		FString UserId;

protected:
	/** Deprecated way of storing items, this is read in but not saved out */
	UPROPERTY()
		TArray<FPrimaryAssetId> InventoryItems_DEPRECATED;

	/** What LatestVersion was when the archive was saved */
	UPROPERTY()
		int32 SavedDataVersion;

	/** Overridden to allow version fixups */
	virtual void Serialize(FArchive& Ar) override;
	
};
