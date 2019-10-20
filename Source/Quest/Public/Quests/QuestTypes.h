// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EQuestTypes : uint8
{
	/** A quest that is critical to the main story */
	MAIN_QUEST,

	/** An optional quest that the player need not complete */
	SIDE_QUEST,
};
