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
	IT_Ring UMETA(DisplayName = "Ring")


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

	/** User-visible long description */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		FText ItemDescription;

	/** User-visible text describing what to do with the item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
		FString UseText;

	/** Icon to display */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		FSlateBrush ItemIcon;

	/** Is the item usable? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		bool bIsUsable;

	/** Ability to trigger upon consuming this item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		TSubclassOf<UQuestGameplayAbility> GrantedAbility;

	/** Can the item be stacked with other identical items in a single inventory slot? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		bool bCanBeStacked;

	/** Can the item be discarded?  (Don't throw away essential quest items) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		bool bCanBeThrownAway;

};
