// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"

/** Order target types */

UENUM(BlueprintType)
enum class EQuestOrderTargetType : uint8
{
	/** No target type; typically when order applies to self */
	NONE,

	/** Order takes an actor as a target */
	ACTOR,

	/** Order takes a vector as a target location */
	LOCATION,

	/** Order takes a vector as a target direction from the character */
	DIRECTION,

	/** Cannot be activated */
	PASSIVE
};