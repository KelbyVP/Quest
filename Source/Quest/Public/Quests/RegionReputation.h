// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "QuestRegion.h"
#include "RegionReputation.generated.h"


/**
 * Holds information about the player's reputation in a region
 */

USTRUCT(BlueprintType)
struct QUEST_API FRegionReputation
{
	GENERATED_BODY()

	/** The region */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RegionReputation")
		EQuestRegion Region;

	/** The player's reputation points in this region */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RegionReputation")
		int32 ReputationPoints;
};
