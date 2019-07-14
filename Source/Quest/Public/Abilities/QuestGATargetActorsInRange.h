// Copyright 2019 Kelby Van Patten

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "QuestGATargetActorsInRange.generated.h"

/**
 * 
 */
UCLASS()
class QUEST_API AQuestGATargetActorsInRange : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	AQuestGATargetActorsInRange();
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "FindActorsInRange")
	FVector TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "FindActorsInRange")
	float TargetRadius;
	
};
