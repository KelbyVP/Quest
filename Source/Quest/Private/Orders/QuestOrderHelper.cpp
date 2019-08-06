// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestOrderHelper.h"
#include "GameFramework/Actor.h"
#include "QuestAbilitySystemHelper.h"
#include "QuestCharacterBase.h"
#include "QuestGlobalTags.h"
#include "QuestOrder.h"
#include "Kismet/KismetSystemLibrary.h"

bool UQuestOrderHelper::CanObeyOrder(TSoftClassPtr<UQuestOrder> OrderType, const AActor* OrderedActor, int32 Index)
{
	return CanObeyOrder(OrderType, OrderedActor, Index, nullptr);
}

bool UQuestOrderHelper::CanObeyOrder(TSoftClassPtr<UQuestOrder> OrderType, const AActor* OrderedActor, int32 Index, 
	FQuestOrderErrorTags* OutErrorTags)
{
	if (OrderType == nullptr || !IsValid(OrderedActor))
	{
		return false;
	}

	if (!OrderType.IsValid())
	{
		OrderType.LoadSynchronous();
	}

	const UQuestOrder* Order = OrderType->GetDefaultObject<UQuestOrder>();
	const UAbilitySystemComponent* AbilitySystem = OrderedActor->FindComponentByClass<UAbilitySystemComponent>();
	if (AbilitySystem != nullptr)
	{
		FQuestOrderTagRequirements TagRequirements;
		Order->GetTagRequirements(OrderedActor, Index, TagRequirements);

		FGameplayTagContainer OrderedActorTags;
		AbilitySystem->GetOwnedGameplayTags(OrderedActorTags);

		if (OutErrorTags != nullptr)
		{
			if (!UQuestAbilitySystemHelper::DoesSatisfyTagRequirementsWithResult(
				OrderedActorTags, TagRequirements.SourceRequiredTags, TagRequirements.SourceBlockedTags,
				OutErrorTags->MissingTags, OutErrorTags->BlockingTags))
			{
				return false;
			}
		}
		else
		{
			if (!UQuestAbilitySystemHelper::DoesSatisfyTagRequirements(
			OrderedActorTags, TagRequirements.SourceRequiredTags, TagRequirements.SourceBlockedTags))
			{
				return false;
			}
		}
	}
	return Order->CanObeyOrder(OrderedActor, Index, OutErrorTags);
}

EQuestOrderTargetType UQuestOrderHelper::GetTargetType(TSoftClassPtr<UQuestOrder> OrderType, 
	const AActor* OrderedActor /*= nullptr*/, int32 Index /*= -1*/)
{
	if (OrderType == nullptr)
	{
		return EQuestOrderTargetType::NONE;
	}
	if (!OrderType.IsValid())
	{
		OrderType.LoadSynchronous();
	}
	return OrderType->GetDefaultObject<UQuestOrder>()->GetTargetType(OrderedActor, Index);
}

bool UQuestOrderHelper::CanOrderBeConsideredAsSucceeded(TSoftClassPtr<UQuestOrder> OrderType, const AActor* OrderedActor, const FQuestOrderTargetData& TargetData, int32 Index /*= -1*/)
{
	if (OrderType == nullptr || !IsValid(OrderedActor))
	{
		return false;
	}
	if (!OrderType.IsValid())
	{
		OrderType.LoadSynchronous();
	}

	const UQuestOrder* Order = OrderType->GetDefaultObject<UQuestOrder>();
	const UAbilitySystemComponent* AbilitySystem = OrderedActor->FindComponentByClass<UAbilitySystemComponent>();

	check(AbilitySystem != nullptr);

	FQuestOrderTagRequirements TagRequirements;
	Order->GetSuccessTagRequirements(OrderedActor, Index, TagRequirements);

	FGameplayTagContainer OrderedActorTags;
	AbilitySystem->GetOwnedGameplayTags(OrderedActorTags);

	if (!UQuestAbilitySystemHelper::DoesSatisfyTagRequirements(
		OrderedActorTags, 
		TagRequirements.SourceRequiredTags, 
		TagRequirements.SourceBlockedTags))
	{
		return false;
	}

	if (!UQuestAbilitySystemHelper::DoesSatisfyTagRequirements(
		TargetData.TargetTags,
		TagRequirements.TargetRequiredTags,
		TagRequirements.TargetBlockedTags))
	{
		return false;
	}
	return true;
}

bool UQuestOrderHelper::IsValidTarget(TSoftClassPtr<UQuestOrder> OrderType, const AActor* OrderedActor, const FQuestOrderTargetData& TargetData, int32 Index /*= -1*/)
{
	return IsValidTarget(OrderType, OrderedActor, TargetData, Index, nullptr);
}

bool UQuestOrderHelper::IsValidTarget(TSoftClassPtr<UQuestOrder> OrderType, const AActor* OrderedActor, 
	const FQuestOrderTargetData& TargetData, int32 Index, FQuestOrderErrorTags* OutErrorTags)
{
	if (OrderType == nullptr)
	{
		return false;
	}

	if (!OrderType.IsValid())
	{
		OrderType.LoadSynchronous();
	}

	const UQuestOrder* Order = OrderType->GetDefaultObject<UQuestOrder>();

	EQuestOrderTargetType TargetType = Order->GetTargetType(OrderedActor, Index);
	if (TargetType == EQuestOrderTargetType::ACTOR)
	{
		if (!IsValid(TargetData.Actor))
		{
			if (OutErrorTags != nullptr)
			{
				OutErrorTags->ErrorTags.AddTag(UQuestGlobalTags::AbilityActivationFailure_NoTarget());
			}
			return false;
		}

		FQuestOrderTagRequirements TagRequirements;
		Order->GetTagRequirements(OrderedActor, Index, TagRequirements);

		if (OutErrorTags != nullptr)
		{
			if (!UQuestAbilitySystemHelper::DoesSatisfyTagRequirementsWithResult(
			TargetData.TargetTags,
				TagRequirements.TargetRequiredTags,
				TagRequirements.TargetBlockedTags,
				OutErrorTags->MissingTags,
				OutErrorTags->BlockingTags))
			{
				return false;
			}
			else
			{
				if (!UQuestAbilitySystemHelper::DoesSatisfyTagRequirements(
					TargetData.TargetTags,
					TagRequirements.TargetRequiredTags,
					TagRequirements.TargetBlockedTags))
				{
					return false;
				}
			}
		}
	}
	return Order->IsValidTarget(OrderedActor, TargetData, Index, OutErrorTags);
}

FQuestOrderTargetData UQuestOrderHelper::CreateOrderTargetData(const AActor* OrderedActor, 
	AActor* TargetActor, const FVector& TargetLocation /*= FVector::ZeroVector*/)
{
	FQuestOrderTargetData TargetData;
	TargetData.Actor = TargetActor;
	TargetData.Location = TargetLocation;

	if (TargetActor == nullptr)
	{
		return TargetData;
	}

	FGameplayTagContainer SourceTags;
	FGameplayTagContainer TargetTags;

	AQuestCharacterBase* QuestTargetActor = Cast<AQuestCharacterBase>(TargetActor);
	if (QuestTargetActor)
	{
		QuestTargetActor->GetOwnedGameplayTags(TargetTags);
	}
	else
	{
		TargetTags = FGameplayTagContainer();
	}
	TargetData.TargetTags = TargetTags;
	return TargetData;
	
}

UBehaviorTree* UQuestOrderHelper::GetBehaviorTree(TSoftClassPtr<UQuestOrder> OrderType)
{
	if (OrderType == nullptr)
	{
		return nullptr;
	}

	if (!OrderType.IsValid())
	{
		OrderType.LoadSynchronous();
	}
	return OrderType->GetDefaultObject<UQuestOrder>()->GetBehaviorTree();
}

EQuestOrderProcessPolicy UQuestOrderHelper::GetOrderProcessPolicy(TSoftClassPtr<UQuestOrder> OrderType, const AActor* OrderedActor, int32 Index /* = -1*/)
{
	if (!IsValid(OrderedActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestOrderHelper::GetOrderProcessPolicy: The actor is invalid."));
		return EQuestOrderProcessPolicy::CAN_BE_CANCELLED;
	}
	if (OrderType == nullptr)
	{
		return EQuestOrderProcessPolicy::CAN_BE_CANCELLED;
	}
	if (!OrderType.IsValid())
	{
		OrderType.LoadSynchronous();
	}
	return OrderType->GetDefaultObject<UQuestOrder>()->GetOrderProcessPolicy(OrderedActor, Index);

}

void UQuestOrderHelper::GetOrderTagRequirements(TSoftClassPtr<UQuestOrder> OrderType, const AActor* OrderedActor, 
	int32 Index, FQuestOrderTagRequirements& OutTagRequirements)
{
	if (OrderType == nullptr)
	{
		UE_LOG(LogTemp, Error, 
			TEXT("UQuestOrderHelper::GetOrderTagRequirements: Error: parameter 'OrderType' was 'nullptr'."));
			return;
	}
	if (!OrderType.IsValid())
	{
		OrderType.LoadSynchronous();
	}
	return OrderType->GetDefaultObject<UQuestOrder>()->GetTagRequirements(OrderedActor, Index, OutTagRequirements);
}

bool UQuestOrderHelper::ShouldRestartBehaviorTree(TSoftClassPtr<UQuestOrder> OrderType)
{
	if (OrderType == nullptr)
	{
		return true;
	}

	if (!OrderType.IsValid())
	{
		OrderType.LoadSynchronous();
	}
	return OrderType->GetDefaultObject<UQuestOrder>()->ShouldRestartBehaviorTree();
}
