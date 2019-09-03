// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestAttackOrder.h"

UQuestAttackOrder::UQuestAttackOrder()
{
	TargetType = EQuestOrderTargetType::ACTOR;
	TargetScoringMethod = EQuestOrderTargetScoringMethod::CHOOSE_CLOSEST_ADVERSARY_TARGET;
	TargetAcquisitionRange = 2000.f;
	Range = 1000.f;
}
