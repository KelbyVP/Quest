// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestSpells.generated.h"

class UGameplayAbility;

UENUM(BlueprintType)
enum class ESpellType : uint8
{
	IT_Paladin UMETA(DisplayName = "Paladin"),
	IT_Priest UMETA(DisplayName = "Priest"),
	IT_Wizard UMETA(DisplayName = "Wizard"),
	IT_Other UMETA(DisplayName = "Other")
};

USTRUCT(BlueprintType)
struct FMasterSpellStruct
{
public:
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MasterSpellStruct")
		TSubclassOf<class UQuestGameplayAbility> Spell;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MasterSpellStruct")
		bool bIsDefaultSpell;

	FMasterSpellStruct()
	{
	};
};

 /** This base class holds an inventory of all available spells for a given spell type */
 /** Since this is a base class, do not blueprint this class directly */
UCLASS()
class QUEST_API UQuestSpells : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	/** The type of spell -- Wizard, Priest, or Paladin */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Spell)
		ESpellType SpellTypeEnum;

	/** Array of spells */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		TArray<FMasterSpellStruct> Spells;
};
