// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "QuestGATargetActorClicked.generated.h"

class UGameplayAbility;
class AQuestPlayerController;

/**
 * 
 */
UCLASS()
class QUEST_API AQuestGATargetActorClicked : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()


public:

		AQuestGATargetActorClicked();

		// Override parent function to begin targeting by setting the relevant ability
		virtual void StartTargeting(UGameplayAbility* Ability) override;

		// Called when destroyed 
		virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

		// Override parent function to see what actor we are clicking on
		virtual void ConfirmTargetingAndContinue() override;

		AQuestPlayerController* PlayerController;
};
