// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestAbilitySystemComponent.h"

UQuestAbilitySystemComponent::UQuestAbilitySystemComponent()
{
	PrimaryComponentTick.bTickEvenWhenPaused = true;

}

void UQuestAbilitySystemComponent::OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnRemoveAbility(AbilitySpec);
}
