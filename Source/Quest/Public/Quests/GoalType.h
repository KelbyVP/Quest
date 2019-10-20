// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGoalType : uint8
{
	/** A custom goal */
	CUSTOM,

	/** A goal to kill one or more characters */
	KILL,

	/** A goal to obtain an item */
	OBTAIN,

	/** A goal to talk to a character */
	TALK,

};
