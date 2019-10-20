// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EQuestStatus : uint8
{
	/** A quest that the player is currently working on */
	CURRENT,

	/** A quest that the player has completed */
	COMPLETED,

	/** A quest that the player has failed to complete */
	FAILED,
};
