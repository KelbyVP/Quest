// Copyright 2019 Kelby Van Patten

#pragma once

#include "CoreMinimal.h"
#include "Items/QuestItem.h"
#include "NameTypes.h"
#include "QuestWeaponItem.generated.h"

class USkeletalMesh;
class UQuestAttackOrder;

/** Describes whether the weapon is melee or ranged */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	IT_Melee UMETA(DisplayName = "Melee"),
	IT_Ranged UMETA(DisplayName = "Ranged")
};

/**
 * 
 */
UCLASS()
class QUEST_API UQuestWeaponItem : public UQuestItem
{
	GENERATED_BODY()

public:

	UQuestWeaponItem();

	/** The socket on the character's skeletal mesh where the weapon will attach */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		FName SocketName;

	/** The static mesh of the weapon, determines its appearance */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		USkeletalMesh *WeaponMesh;

	/** The location where the weapon will spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		FTransform RelativeSpawnLocation;

	/** Whether the weapon is melee or ranged */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		TSoftClassPtr<UQuestAttackOrder> AttackOrder;



	
};
