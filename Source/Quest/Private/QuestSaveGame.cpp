// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSaveGame.h"
#include "QuestGameInstanceBase.h"

void UQuestSaveGame::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	if (Ar.IsLoading() && SavedDataVersion != EQuestSaveGameVersion::LatestVersion)
	{
		if (SavedDataVersion < EQuestSaveGameVersion::AddedItemData)
		{
			// Convert from list to item data map
			for (const FPrimaryAssetId& ItemId : InventoryItems_DEPRECATED)
			{
				InventoryData.Add(ItemId, FQuestItemData(1, 1));
			}

			InventoryItems_DEPRECATED.Empty();
		}

		SavedDataVersion = EQuestSaveGameVersion::LatestVersion;
	}
}
