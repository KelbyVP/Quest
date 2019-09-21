// Copyright 2019 Kelby Van Patten

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "QuestDamageExecutionCalculation.generated.h"

class AQuestCharacterBase;
/**
 * 
 */
UCLASS()
class QUEST_API UQuestDamageExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	// Default constructor
		UQuestDamageExecutionCalculation();


protected:
	int8 GetAttackRollTagModifiers(AQuestCharacterBase* SourceQuestCharacter, int8 AttackTry) const;

};
