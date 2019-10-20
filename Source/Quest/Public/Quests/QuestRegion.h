// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EQuestRegion : uint8
{
	/** The player character's home land */
	ELGONTAAR,

	/** A fiery world */
	BRIMTHACOREAL,

	/** An icy world */
	TAALGIOHOVAN,

	/** ??? */
	BRIMASTUR,

	/** TODO:  NEED SEVERAL MORE WORLDS */

	/** TODO:  Do I need a region that is not specific to a region for quests that span multiple regions? */
};
