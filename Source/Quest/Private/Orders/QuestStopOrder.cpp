// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestStopOrder.h"

UQuestStopOrder::UQuestStopOrder()
{
	TargetType = EQuestOrderTargetType::NONE;

}

bool UQuestStopOrder::AreAutoOrdersAllowedDuringOrder() const
{
	return true;
}
