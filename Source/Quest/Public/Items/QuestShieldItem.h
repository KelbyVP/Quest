// Copyright 2019 Kelby Van Patten

#pragma once

#include "CoreMinimal.h"
#include "Items/QuestItem.h"
#include "QuestShieldItem.generated.h"

class USkeletalMesh;
/**
 * 
 */
UCLASS()
class QUEST_API UQuestShieldItem : public UQuestItem
{
	GENERATED_BODY()
	
public:
	UQuestShieldItem();

	/** The socket on the character's skeletal mesh where the shield will attach */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		FName SocketName;

	/** The static mesh of the shield, determines its appearance */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		USkeletalMesh* ShieldMesh;

	/** The location where the weapon will spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		FTransform RelativeSpawnLocation;
};
