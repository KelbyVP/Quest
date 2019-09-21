// Copyright 2019 Kelby Van Patten


#include "QuestDamageExecutionCalculation.h"
#include "QuestAttributeSet.h"
#include "QuestCharacterBase.h"
#include "QuestGlobalTags.h"



UQuestDamageExecutionCalculation::UQuestDamageExecutionCalculation()
{}

int8 UQuestDamageExecutionCalculation::GetAttackRollTagModifiers(AQuestCharacterBase* SourceQuestCharacter, int8 AttackTry) const
{
	int8 AttackRollTagAdjustment = 0;

	// Adjust if attacking character is blessed
	if (SourceQuestCharacter->DoesCharacterHaveTag(UQuestGlobalTags::Status_Blessed()))
	{
		AttackRollTagAdjustment += FMath::RandRange(1, 4);
	}

	return AttackRollTagAdjustment;
}

