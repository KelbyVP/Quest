// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGoalStatus : uint8
{
	/** A goal that the player is currently working on */
	CURRENT,

	/** A goal that the player has successfully completed */
	SUCCEEDED,

	/** A goal that the player has failed to complete */
	FAILED,
};
