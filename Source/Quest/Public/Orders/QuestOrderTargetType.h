// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"

/**
 *	Type of target that an order can have
 */

UENUM(BlueprintType)
enum class EQuestOrderTargetType : uint8
{
	/** No target type (eg. apply to self) */
	NONE,

	/** Order targets an actor */
	ACTOR,

	/** Order targets a vector location */
	LOCATION,

	/** Order targets a vector location which, along with the character's location, creates a direction */
	DIRECTION,

	/** Cannot activate */
	// TODO: do we need this one?
	PASSIVE,
};