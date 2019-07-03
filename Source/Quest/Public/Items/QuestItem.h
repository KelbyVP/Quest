// Copyright 2019 Kelby Van Patten

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Texture2D.h"
#include "Runtime/SlateCore/Public/Styling/SlateBrush.h"
#include "QuestItem.generated.h"

class UQuestGameplayAbility;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	IT_Potion UMETA(DisplayName = "Potion"),
	IT_Weapon UMETA(DisplayName = "Weapon"),
	IT_Ring UMETA(DisplayName = "Ring"),
	IT_Material UMETA(DisplayName = "Material"),
	IT_Amulet UMETA(DisplayName = "Amulet"),
	IT_Shield UMETA(DisplayName = "Shield")
};

UENUM(BlueprintType)
enum class ERarity : uint8
{
	IT_Common UMETA(DisplayName = "Common"),
	IT_Uncommon UMETA(DisplayName = "Uncommon"),
	IT_Rare UMETA(DisplayName = "Rare"),
	IT_VeryRare UMETA(DisplayName = "Very Rare"),
	IT_Legendary UMETA(DisplayName = "Legendary"),
	IT_Unique UMETA(DisplayName = "Unique")
};

/** Base class for all items; do not blueprint this class directly */
UCLASS(Abstract, BlueprintType)
class QUEST_API UQuestItem : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	/** Type of this item, set in native parent class */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
		FPrimaryAssetType ItemType;

	/** ENUM type of this item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		EItemType ItemTypeEnum;

	/** User-visible short name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		FText ItemName;

	/** Name without caps or white spaces; used for sorting */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		FText SortName;

	/** User-visible long description */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		FText ItemDescription;

	/** User-visible text describing what to do with the item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
		FString UseText;

	/** Icon to display */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		FSlateBrush ItemIcon;

	/** Value of the item, which will determine price in gold at which it can be bought and sold */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		float Value;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		ERarity Rarity;

	/** Is the item usable? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		bool bIsUsable;

	/** Can the item be equipped by a character? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		bool bCanBeEquipped;

	/** Ability to trigger upon consuming this item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		TSubclassOf<UQuestGameplayAbility> GrantedAbility;

	/** Can the item be stacked with other identical items in a single inventory slot? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		bool bCanBeStacked;

	/** Can the item be discarded?  (Don't throw away essential quest items) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		bool bCanBeThrownAway;

	/** Can the item be crafted? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		bool bCanBeCrafted;

	/** If the item can be crafted, these are the items that are necessary to make it */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		TArray<UQuestItem*> Recipe;
};
