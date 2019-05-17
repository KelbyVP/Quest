// Copyright 2019 Kelby Van Patten


#include "QuestGameInstanceBase.h"
#include "QuestAssetManager.h"
#include "QuestSaveGame.h"
#include "QuestItem.h"
#include "Kismet/GameplayStatics.h"

UQuestGameInstanceBase::UQuestGameInstanceBase()
	: SaveSlot(TEXT("SaveGame"))
	, SaveUserIndex(0)
{}

void UQuestGameInstanceBase::AddDefaultInventory(UQuestSaveGame* SaveGame, bool bRemoveExtra /*= false*/)
{
	// If we want to remove extra, clear out the existing inventory
	if (bRemoveExtra)
	{
		SaveGame->InventoryData.Reset();
	}

	// Now add the default inventory, this only adds if not already in the inventory
	for (const TPair<FPrimaryAssetId, FQuestItemData>& Pair : DefaultInventory)
	{
		if (!SaveGame->InventoryData.Contains(Pair.Key))
		{
			SaveGame->InventoryData.Add(Pair.Key, Pair.Value);
		}
	}
}

bool UQuestGameInstanceBase::IsValidItemSlot(FQuestItemSlot ItemSlot) const
{
	if (ItemSlot.IsValid())
	{
		const int32* FoundCount = ItemSlotsPerType.Find(ItemSlot.ItemType);

		if (FoundCount)
		{
			return ItemSlot.SlotNumber < *FoundCount;
		}
	}
	return false;
}

UQuestSaveGame* UQuestGameInstanceBase::GetCurrentSaveGame()
{
	return CurrentSaveGame;
}

void UQuestGameInstanceBase::SetSavingEnabled(bool bEnabled)
{
	bSavingEnabled = bEnabled;
}

bool UQuestGameInstanceBase::LoadOrCreateSaveGame()
{
	// Drop reference to old save game, this will GC out
	CurrentSaveGame = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(SaveSlot, SaveUserIndex) && bSavingEnabled)
	{
		CurrentSaveGame = Cast<UQuestSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, SaveUserIndex));
	}

	if (CurrentSaveGame)
	{
		// Make sure it has any newly added default inventory
		AddDefaultInventory(CurrentSaveGame, false);

		return true;
	}
	else
	{
		// This creates it on demand
		CurrentSaveGame = Cast<UQuestSaveGame>(UGameplayStatics::CreateSaveGameObject(UQuestSaveGame::StaticClass()));

		AddDefaultInventory(CurrentSaveGame, true);

		return false;
	}
}

bool UQuestGameInstanceBase::WriteSaveGame()
{
	if (bSavingEnabled)
	{
		return UGameplayStatics::SaveGameToSlot(GetCurrentSaveGame(), SaveSlot, SaveUserIndex);
	}
	return false;
}

void UQuestGameInstanceBase::ResetSaveGame()
{
	bool bWasSavingEnabled = bSavingEnabled;
	bSavingEnabled = false;
	LoadOrCreateSaveGame();
	bSavingEnabled = bWasSavingEnabled;
}
