// Copyright 2019 Kelby Van Patten


#include "QuestAssetManager.h"
#include "QuestItem.h"
#include "AbilitySystemGlobals.h"

void UQuestAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}

const FPrimaryAssetType UQuestAssetManager::PotionItemType = TEXT("Potion");
const FPrimaryAssetType UQuestAssetManager::SkillItemType = TEXT("Skill");
const FPrimaryAssetType UQuestAssetManager::WeaponItemType = TEXT("Weapon");


UQuestAssetManager& UQuestAssetManager::Get()
{
	UQuestAssetManager* This = Cast<UQuestAssetManager>(GEngine->AssetManager);

	if (This)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestAssetManager pointer created!"));
		return *This;
	}
	else
	{
		UE_LOG(LogQuest, Fatal, TEXT("InvalidAssetManager in DefaultEngine.ini, must be QuestAssetManager!"));
		return *NewObject<UQuestAssetManager>(); // never calls this
	}

}

UQuestItem* UQuestAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	// This does a synchronous load and may hitch
	UQuestItem* LoadedItem = Cast<UQuestItem>(ItemPath.TryLoad());

	if (bLogWarning && LoadedItem == nullptr)
	{
		UE_LOG(LogQuest, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetId.ToString());
	}

	return LoadedItem;
}
