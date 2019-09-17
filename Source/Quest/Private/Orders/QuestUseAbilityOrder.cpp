// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestUseAbilityOrder.h"

UQuestUseAbilityOrder::UQuestUseAbilityOrder()
{

}

UQuestUseAbilityOrder::UQuestUseAbilityOrder(TSubclassOf<class UQuestGameplayAbility> InAbility)
{
	Ability = InAbility;
}


