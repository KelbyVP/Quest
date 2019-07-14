// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QuestGameplayAbility.h"
#include "QuestFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS(meta = (ScriptName = "QuestFunctionLibrary"))
class QUEST_API UQuestFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

		UFUNCTION(BlueprintCallable, Category = "QuestFunctionLibrary")
		static void ApplyGameplayEffectSpecHandleToTargetDataHandleSpecHandle(const FGameplayEffectSpecHandle& GESpecHandle, UPARAM(ref) FGameplayAbilityTargetDataHandle& TargetDataHandle);

};
