// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

UENUM(BlueprintType)
enum class EQuestOrderTargetScoringMethod : uint8
{
	/** Choose closest enemy target */
	CHOOSE_CLOSEST_ADVERSARY_TARGET,

	/** Choose enemy leader */
	CHOOSE_ADVERSARY_LEADER,

	/** Choose most powerful adversary */
	CHOOSE_MOST_POWERFUL_ADVERSARY_TARGET,

	/** Choose enemy target that is a spellcaster if possible*/
	CHOOSE_ADVERSARY_SPELLCASTER,

	/** Choose allied leader */
	CHOOSE_ALLIED_LEADER,
};
