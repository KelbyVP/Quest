// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GoalsToCompleteLater.generated.h"

class AQuestQuestBase;
/**
 *  Holds information about quests that need to be completed but cannot yet while a for loop is running
 */

USTRUCT(BlueprintType)
struct QUEST_API FGoalsToCompleteLater

{
	GENERATED_BODY()

	AQuestQuestBase* Quest;
	int32 Index;
};