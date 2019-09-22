// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/QuestSpellDamageExecCalcBase.h"
#include "QuestBurningDamageExecCalc.generated.h"

/**
 * 
 */
UCLASS()
class QUEST_API UQuestBurningDamageExecCalc : public UQuestSpellDamageExecCalcBase
{
	GENERATED_BODY()

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
