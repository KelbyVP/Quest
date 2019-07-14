// Copyright 2019 Kelby Van Patten, All Rights Reserved

#include "QuestFunctionLibrary.h"



void UQuestFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetDataHandleSpecHandle(const FGameplayEffectSpecHandle& GESpecHandle, UPARAM(ref) FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	for (TSharedPtr<FGameplayAbilityTargetData> Data : TargetDataHandle.Data)
	{
		Data->ApplyGameplayEffectSpec(*GESpecHandle.Data.Get());
	}
}
