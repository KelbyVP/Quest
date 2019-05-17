// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Quest.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "QuestInventoryInterface.h"
#include "QuestPlayerController.generated.h"

class AQuestCharacter;
class AQuestCharacterBase;

UCLASS()
class QUEST_API AQuestPlayerController : public APlayerController, public IQuestInventoryInterface
{
	GENERATED_BODY()

public:
	AQuestPlayerController();
	virtual void BeginPlay() override;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(FHitResult &Hit);

	/** Input handlers for SetDestination action. */
	void OnSetTargetPressed();
	void OnSetTargetReleased();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Quest Player Controller")
		FVector DestinationLocation;

public:
	AQuestCharacterBase* PawnClicked;

	/**	Set whether the controller can move the character
	*	(false if character is trying to cast spell, attack from range, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestPlayerController")
	bool bControllerCanMoveCharacter;

	/** Map of all items owned by this player, from definition to data */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
		TMap<UQuestItem*, FQuestItemData> InventoryData;

	/** Map of slot, from type/num to item, initialized from ItemSlotsPerType on QuestGameInstanceBase
	*  TODO: Create QuestGameInstanceBase */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
		TMap<FQuestItemSlot, UQuestItem*> SlottedItems;

	/** Delegate called when an inventory item has been added or removed */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
		FOnInventoryItemChanged OnInventoryItemChanged;

	/** Native version above, called before BP delegate */
	FOnInventoryItemChangedNative OnInventoryItemChangedNative;

	/** Delegate called when an inventory slot has changed */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
		FOnSlottedItemChanged OnSlottedItemChanged;

	/** Called after the inventory was changed and we notified all delegates */
	UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
		void InventoryItemChanged(bool bAdded, UQuestItem* Item);

	/** Called after an item was equipped and we notified all delegates */
	UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
		void SlottedItemChanged(FQuestItemSlot ItemSlot, UQuestItem* Item);

	/** Native Version above, called before BP delegate */
	FOnSlottedItemChangedNative OnSlottedItemChangedNative;

	/** Delegate called when the inventory has been loaded/reloaded */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
		FOnInventoryLoaded OnInventoryLoaded;

	/** Native version above, called before BP delegate */
	FOnInventoryLoadedNative OnInventoryLoadedNative;

	/** Adds a new inventory item, will add it to an empty slot if possible. If the item supports count you can add more than one count. It will also update the level when adding if required */
	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool AddInventoryItem(UQuestItem* NewItem, int32 ItemCount = 1, int32 ItemLevel = 1, bool bAutoSlot = true);

	/** Remove an inventory item, will also remove from slots. A remove count of <= 0 means to remove all copies */
	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool RemoveInventoryItem(UQuestItem* RemovedItem, int32 RemoveCount = 1);

	/** Returns all inventory items of a given type. If none is passed as type it will return all */
	UFUNCTION(BlueprintCallable, Category = Inventory)
		void GetInventoryItems(TArray<UQuestItem*>& Items, FPrimaryAssetType ItemType);

	/** Returns number of instances of this item found in the inventory. This uses count from GetItemData */
	UFUNCTION(BlueprintPure, Category = Inventory)
		int32 GetInventoryItemCount(UQuestItem* Item) const;

	/** Returns the item data associated with an item. Returns false if none found */
	UFUNCTION(BlueprintPure, Category = Inventory)
		bool GetInventoryItemData(UQuestItem* Item, FQuestItemData& ItemData) const;

	/** Sets slot to item, will remove from other slots if necessary. If passing null this will empty the slot */
	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool SetSlottedItem(FQuestItemSlot ItemSlot, UQuestItem* Item);

	/** Returns item in slot, or null if empty */
	UFUNCTION(BlueprintPure, Category = Inventory)
		UQuestItem* GetSlottedItem(FQuestItemSlot ItemSlot) const;

	/** Returns all slotted items of a given type. If none is passed as type it will return all */
	UFUNCTION(BlueprintCallable, Category = Inventory)
		void GetSlottedItems(TArray<UQuestItem*>& Items, FPrimaryAssetType ItemType, bool bOutputEmptyIndexes);

	/** Fills in any empty slots with items in inventory */
	UFUNCTION(BlueprintCallable, Category = Inventory)
		void FillEmptySlots();

	/** Manually save the inventory, this is called from add/remove functions automatically */
	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool SaveInventory();

	/** Loads inventory from save game on game instance, this will replace arrays */
	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool LoadInventory();

	// Implement IQuestInventoryInterface
	virtual const TMap<UQuestItem*, FQuestItemData>& GetInventoryDataMap() const override
	{
		return InventoryData;
	}
	virtual const TMap<FQuestItemSlot, UQuestItem*>& GetSlottedItemMap() const override
	{
		return SlottedItems;
	}
	virtual FOnInventoryItemChangedNative& GetInventoryItemChangedDelegate() override
	{
		return OnInventoryItemChangedNative;
	}
	virtual FOnSlottedItemChangedNative& GetSlottedItemChangedDelegate() override
	{
		return OnSlottedItemChangedNative;
	}
	virtual FOnInventoryLoadedNative& GetInventoryLoadedDelegate() override
	{
		return OnInventoryLoadedNative;
	}

protected:
	/** Auto slots a specific item, returns true if anything changed */
	bool FillEmptySlotWithItem(UQuestItem* NewItem);

	/** Calls the inventory update callbacks */
	void NotifyInventoryItemChanged(bool bAdded, UQuestItem* Item);
	void NotifySlottedItemChanged(FQuestItemSlot ItemSlot, UQuestItem* Item);
	void NotifyInventoryLoaded();



private:
	AQuestCharacter* ControlledCharacter;

};


