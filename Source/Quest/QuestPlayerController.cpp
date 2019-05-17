// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "QuestPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "QuestCharacter.h"
#include "QuestItem.h"
#include "QuestGameInstanceBase.h"
#include "QuestSaveGame.h"
#include "Engine/World.h"

AQuestPlayerController::AQuestPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bControllerCanMoveCharacter = true;
	ControlledCharacter = Cast<AQuestCharacter>(GetPawn());
}

void AQuestPlayerController::BeginPlay()
{
	// Load inventory off save game before starting play
	UE_LOG(LogTemp, Warning, TEXT("Native begin play called from controller!"))
	LoadInventory();
	Super::BeginPlay();
}

void AQuestPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
	if (!ControlledCharacter)
	{
		ControlledCharacter = Cast<AQuestCharacter>(GetPawn());
	}
	if (ControlledCharacter->TargetCharacter)
	{	
		if (ControlledCharacter->bIsTargetCharacterWithinInteractionSphere)
		{
			if (!ControlledCharacter->TargetCharacter->bIsDead)
			{
				if (ControlledCharacter->TargetCharacter->bIsHostile)
				{
					ControlledCharacter->MeleeAttack();
				}
				else
				{
					// TODO:  initiate dialogue if the target is not dead and is not hostile
				}
			}
			else
			{
				// TODO:  loot the body if the target is dead
			}
		}
	}
}

void AQuestPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetTarget", IE_Pressed, this, &AQuestPlayerController::OnSetTargetPressed);
	InputComponent->BindAction("SetTarget", IE_Released, this, &AQuestPlayerController::OnSetTargetReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AQuestPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AQuestPlayerController::MoveToTouchLocation);
}


void AQuestPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (AQuestCharacter* MyPawn = Cast<AQuestCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		 FHitResult Hit;
		 GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit);
		}
	}
}

void AQuestPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult);
	}
}

void AQuestPlayerController::SetNewMoveDestination(FHitResult &Hit)
{
	FVector DestLocation = Hit.ImpactPoint;
	if (Hit.bBlockingHit)
	{
		AActor* ActorClicked;
		ActorClicked = Hit.GetActor();
		PawnClicked = Cast<AQuestCharacterBase>(ActorClicked);
		ControlledCharacter = Cast<AQuestCharacter>(GetPawn());

		if (ControlledCharacter)
		{
			//  If we clicked on a character, set it as the target character
			if (PawnClicked)
			{
				float const Distance = FVector::Dist(PawnClicked->GetActorLocation(), ControlledCharacter->GetActorLocation());
				ControlledCharacter->TargetCharacter = PawnClicked;

				// if we are out of range, move to the target character

				if (Distance > ControlledCharacter->InteractionSphereRadius)
				{
					ControlledCharacter->bIsTargetCharacterWithinInteractionSphere = false;
					UAIBlueprintHelperLibrary::SimpleMoveToActor(this, ControlledCharacter->TargetCharacter);
				}
				else
				{
					ControlledCharacter->bIsTargetCharacterWithinInteractionSphere = true;
				}
			}
			// If we did not click on a character, move unless we're too close for the animation to play correctly
			else
			{
				ControlledCharacter->TargetCharacter = nullptr;
				float const Distance = FVector::Dist(DestLocation, ControlledCharacter->GetActorLocation());
				if ((Distance > 120.0f))
				{
					UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
					DestinationLocation = DestLocation;
				}
			}
		}
	}
}

void AQuestPlayerController::OnSetTargetPressed()
{
	if (bControllerCanMoveCharacter)
	{
		// set flag to keep updating destination until released
		bMoveToMouseCursor = true;
	}

}

void AQuestPlayerController::OnSetTargetReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

bool AQuestPlayerController::AddInventoryItem(UQuestItem* NewItem, int32 ItemCount /*= 1*/, int32 ItemLevel /*= 1*/, bool bAutoSlot /*= true*/)
{
	bool bChanged = false;
	if (!NewItem)
	{
		UE_LOG(LogQuest, Warning, TEXT("AddInventoryItem: Failed trying to add null item!"));
		return false;
	}

	if (ItemCount <= 0 || ItemLevel <= 0)
	{
		UE_LOG(LogQuest, Warning, TEXT("AddInventoryItem: Failed trying to add item %s with negative count or level!"), *NewItem->GetName());
		return false;
	}

	// Find current item data, which may be empty
	FQuestItemData OldData;
	GetInventoryItemData(NewItem, OldData);

	// Find modified data
	FQuestItemData NewData = OldData;
	NewData.UpdateItemData(FQuestItemData(ItemCount, ItemLevel), NewItem->MaxCount, NewItem->MaxLevel);

	if (OldData != NewData)
	{
		// If data changed, need to update storage and call callback
		InventoryData.Add(NewItem, NewData);
		NotifyInventoryItemChanged(true, NewItem);
		bChanged = true;
	}

	if (bAutoSlot)
	{
		// Slot item if required
		bChanged |= FillEmptySlotWithItem(NewItem);
	}

	if (bChanged)
	{
		// If anything changed, write to save game
		SaveInventory();
		return true;
	}
	return false;
}

bool AQuestPlayerController::RemoveInventoryItem(UQuestItem* RemovedItem, int32 RemoveCount /*= 1*/)
{
	if (!RemovedItem)
	{
		UE_LOG(LogQuest, Warning, TEXT("RemoveInventoryItem: Failed trying to remove null item!"));
		return false;
	}

	// Find current item data, which may be empty
	FQuestItemData NewData;
	GetInventoryItemData(RemovedItem, NewData);

	if (!NewData.IsValid())
	{
		// Wasn't found
		return false;
	}

	// If RemoveCount <=0, delete all
	if (RemoveCount <=0)
	{
		NewData.ItemCount = 0;
	}
	else
	{
		NewData.ItemCount -= RemoveCount;
	}

	if (NewData.ItemCount > 0)
	{
		// Update data with new count
		InventoryData.Add(RemovedItem, NewData);
	}
	else
	{
		// Remove item entirely, make sure it is unslotted
		InventoryData.Remove(RemovedItem);

		for (TPair<FQuestItemSlot, UQuestItem*>& Pair : SlottedItems)
		{
			if (Pair.Value == RemovedItem)
			{
				Pair.Value = nullptr;
				NotifySlottedItemChanged(Pair.Key, Pair.Value);
			}
		}
	}

	// If we got this far, there is a change so notify and save
	NotifyInventoryItemChanged(false, RemovedItem);
	SaveInventory();
	return true;
}

void AQuestPlayerController::GetInventoryItems(TArray<UQuestItem*>& Items, FPrimaryAssetType ItemType)
{
	for (const TPair<UQuestItem*, FQuestItemData>& Pair : InventoryData)
	{
		if (Pair.Key)
		{
			FPrimaryAssetId AssetId = Pair.Key->GetPrimaryAssetId();

			// Filters based on item type
			if (AssetId.PrimaryAssetType == ItemType || !ItemType.IsValid())
			{
				Items.Add(Pair.Key);
			}
		}
	}
}

int32 AQuestPlayerController::GetInventoryItemCount(UQuestItem* Item) const
{
	const FQuestItemData* FoundItem = InventoryData.Find(Item);
	if (FoundItem)
	{
		return FoundItem->ItemCount;
	}
	return 0;
}

bool AQuestPlayerController::GetInventoryItemData(UQuestItem* Item, FQuestItemData& ItemData) const
{
	const FQuestItemData* FoundItem = InventoryData.Find(Item);
	if (FoundItem)
	{
		ItemData = *FoundItem;
		return true;
	}
	ItemData = FQuestItemData(0, 0);
	return false;
}

bool AQuestPlayerController::SetSlottedItem(FQuestItemSlot ItemSlot, UQuestItem* Item)
{
	// Iterate entire inventory because we need to remove from old slot
	bool bFound = false;
	for (TPair<FQuestItemSlot, UQuestItem*>& Pair : SlottedItems)
	{
		if (Pair.Key == ItemSlot)
		{
			// Add to new slot
			bFound = true;
			Pair.Value = Item;
			NotifySlottedItemChanged(Pair.Key, Pair.Value);
		}
		else if (Item != nullptr && Pair.Value == Item)
		{
			// if this item was found in another slot, remove it
			Pair.Value = nullptr;
			NotifySlottedItemChanged(Pair.Key, Pair.Value);
		}
	}

	if (bFound)
	{
		SaveInventory();
		return true;
	}
	return false;
}

UQuestItem* AQuestPlayerController::GetSlottedItem(FQuestItemSlot ItemSlot) const
{
	UQuestItem* const* FoundItem = SlottedItems.Find(ItemSlot);
	if (FoundItem)
	{
		return *FoundItem;
	}
	return nullptr;
}

void AQuestPlayerController::GetSlottedItems(TArray<UQuestItem*>& Items, FPrimaryAssetType ItemType, bool bOutputEmptyIndexes)
{
	for (TPair<FQuestItemSlot, UQuestItem*>& Pair : SlottedItems)
	{
		if (Pair.Key.ItemType == ItemType || !ItemType.IsValid())
		{
			Items.Add(Pair.Value);
		}
	}
}

void AQuestPlayerController::FillEmptySlots()
{
	bool bShouldSave = false;
	for (const TPair<UQuestItem*, FQuestItemData>& Pair : InventoryData)
	{
		bShouldSave |= FillEmptySlotWithItem(Pair.Key);
	}
	if (bShouldSave)
	{
		SaveInventory();
	}
}

bool AQuestPlayerController::SaveInventory()
{

	UWorld* World = GetWorld();
	UQuestGameInstanceBase* GameInstance = World ? World->GetGameInstance<UQuestGameInstanceBase>() : nullptr;

	if (!GameInstance)
	{
		return false;
	}

	UQuestSaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	if (CurrentSaveGame)
	{
		// Reset cached data in save game before writing to it
		CurrentSaveGame->InventoryData.Reset();
		CurrentSaveGame->SlottedItems.Reset();

		for (const TPair<UQuestItem*, FQuestItemData>& ItemPair : InventoryData)
		{
			FPrimaryAssetId AssetId;

			if (ItemPair.Key)
			{
				AssetId = ItemPair.Key->GetPrimaryAssetId();
				CurrentSaveGame->InventoryData.Add(AssetId, ItemPair.Value);
			}
		}

		for (const TPair<FQuestItemSlot, UQuestItem*>& SlotPair : SlottedItems)
		{
			FPrimaryAssetId AssetId;

			if (SlotPair.Value)
			{
				AssetId = SlotPair.Value->GetPrimaryAssetId();
			}
			CurrentSaveGame->SlottedItems.Add(SlotPair.Key, AssetId);
		}

		// Now that cache is updated, write to disk
		GameInstance->WriteSaveGame();
		return true;
	}
	return false;
}

bool AQuestPlayerController::LoadInventory()
{
	InventoryData.Reset();
	SlottedItems.Reset();
	// Fill in slots from game instance
	UWorld* World = GetWorld();
	UQuestGameInstanceBase* GameInstance = World ? World->GetGameInstance<UQuestGameInstanceBase>() : nullptr;

	if (!GameInstance)
	{
		return false;
	}

	for (const TPair<FPrimaryAssetType, int32>& Pair : GameInstance->ItemSlotsPerType)
	{
		for (int32 SlotNumber = 0; SlotNumber < Pair.Value; SlotNumber++)
		{
			
			SlottedItems.Add(FQuestItemSlot(Pair.Key, SlotNumber), nullptr);
		}
	}

	UQuestSaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	UQuestAssetManager& AssetManager = UQuestAssetManager::Get();
	if (CurrentSaveGame)
	{
		// Copy from save game into controller data
		bool bFoundAnySlots = false;
		for (const TPair<FPrimaryAssetId, FQuestItemData>& ItemPair : CurrentSaveGame->InventoryData)
		{
			UQuestItem* LoadedItem = AssetManager.ForceLoadItem(ItemPair.Key);

			if (LoadedItem != nullptr)
			{
				InventoryData.Add(LoadedItem, ItemPair.Value);
			}
		}

		for (const TPair<FQuestItemSlot, FPrimaryAssetId>& SlotPair : CurrentSaveGame->SlottedItems)
		{
			if (SlotPair.Value.IsValid())
			{
				UQuestItem* LoadedItem = AssetManager.ForceLoadItem(SlotPair.Value);
				if (GameInstance->IsValidItemSlot(SlotPair.Key) && LoadedItem)
				{
					SlottedItems.Add(SlotPair.Key, LoadedItem);
					bFoundAnySlots = true;
				}
			}
		}

		if (!bFoundAnySlots)
		{
			// Auto slot items as no slots were saved
			FillEmptySlots();
		}

		NotifyInventoryLoaded();

		return true;
	}

	// Load failed but we reset inventory, so need to notify UI
	NotifyInventoryLoaded();
	return false;
}

bool AQuestPlayerController::FillEmptySlotWithItem(UQuestItem* NewItem)
{
	// Look for an empty item slot to fill with this item
	FPrimaryAssetType NewItemType = NewItem->GetPrimaryAssetId().PrimaryAssetType;
	FQuestItemSlot EmptySlot;
	for (TPair<FQuestItemSlot, UQuestItem*>& Pair : SlottedItems)
	{
		if (Pair.Key.ItemType == NewItemType)
		{
			if (Pair.Value == NewItem)
			{
				// Item is already slotted
				return false;
			}
			else if (Pair.Value == nullptr && (!EmptySlot.IsValid() || EmptySlot.SlotNumber > Pair.Key.SlotNumber))
			{
				// We found an empty slot worth filling
				EmptySlot = Pair.Key;
			}
		}
	}
	if (EmptySlot.IsValid())
	{
		SlottedItems[EmptySlot] = NewItem;
		NotifySlottedItemChanged(EmptySlot, NewItem);
		return true;
	}
	return false;
}

void AQuestPlayerController::NotifyInventoryItemChanged(bool bAdded, UQuestItem* Item)
{
	// Notify native before blueprint
	OnInventoryItemChangedNative.Broadcast(bAdded, Item);
	OnInventoryItemChanged.Broadcast(bAdded, Item);

	// Call BP update event
	InventoryItemChanged(bAdded, Item);
}

void AQuestPlayerController::NotifySlottedItemChanged(FQuestItemSlot ItemSlot, UQuestItem* Item)
{
	// Notify native before blueprint
	OnSlottedItemChangedNative.Broadcast(ItemSlot, Item);
	OnSlottedItemChanged.Broadcast(ItemSlot, Item);

	// Call BP update event
	SlottedItemChanged(ItemSlot, Item);
}

void AQuestPlayerController::NotifyInventoryLoaded()
{
	// Notify native before blueprint
	OnInventoryLoadedNative.Broadcast();
	OnInventoryLoaded.Broadcast();
}
