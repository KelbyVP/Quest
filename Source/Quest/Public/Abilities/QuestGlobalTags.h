// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameplayTagContainer.h"
#include "QuestGlobalTags.generated.h"
/**
 *  Global tags
 */
UCLASS()
class QUEST_API UQuestGlobalTags : public UAIBlueprintHelperLibrary
{
	GENERATED_BODY()

public:

	static bool CanTagBeStacked(const FGameplayTag& Tag);

	/** Functions that return a static gameplay tag of the specified type */
	static const FGameplayTag& Relationship_Friendly();
	static const FGameplayTag& Relationship_Hostile();
	static const FGameplayTag& Relationship_Neutral();
	static const FGameplayTag& Relationship_Self();
	static const FGameplayTag& Relationship_Visible();
	static const FGameplayTag& Status_Alive();
};
