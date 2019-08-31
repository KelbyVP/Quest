// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestOrderHelperLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "QuestAbilitySystemHelper.h"
#include "QuestOrder.h"
#include "QuestOrderErrorTags.h"


/** TODO:  The Orders/Ability code sets this up as multiple functions by having the function create the tags and then calling another version
*	that takes a reference to the tags as a parameter.  May need to do that if I need to call a version of the function that returns the tags.\
*/

bool UQuestOrderHelperLibrary::CanObeyOrder(TSoftClassPtr<UQuestOrder> OrderType, const AActor* OrderedActor)
{
	FQuestOrderErrorTags ErrorTags;

	/** Clear all error tags, just to be safe */
	ErrorTags.MissingTags.Reset();
	ErrorTags.BlockingTags.Reset();
	ErrorTags.ErrorTags.Reset();

	if (OrderType == nullptr || !IsValid(OrderedActor))
	{
		return false;
	}

	if (OrderType.IsValid())
	{
		OrderType.LoadSynchronous();
	}

	const UQuestOrder* Order = OrderType->GetDefaultObject<UQuestOrder>();
	const UAbilitySystemComponent* AbilitySystem = OrderedActor->FindComponentByClass<UAbilitySystemComponent>();

	if (AbilitySystem != nullptr)
	{
		FQuestOrderTagRequirements TagRequirements;
		Order->GetTagRequirements(OrderedActor, TagRequirements);

		FGameplayTagContainer OrderedActorTags;
		AbilitySystem->GetOwnedGameplayTags(OrderedActorTags);

		if (!UQuestAbilitySystemHelper::DoesSatisfyTagRequirementsWithResult(
			OrderedActorTags, TagRequirements.SourceTagsRequired, TagRequirements.SourceTagsBlocked,
			ErrorTags.MissingTags, ErrorTags.BlockingTags))
		{
			return false;
		}
		else
		{
			if (!UQuestAbilitySystemHelper::DoesSatisfyTagRequirements(
				OrderedActorTags, TagRequirements.SourceTagsRequired, TagRequirements.SourceTagsBlocked))
			{
				return false;
			}
		}
	}
	return true;
}

EQuestOrderTargetType UQuestOrderHelperLibrary::GetTargetType(TSoftClassPtr<UQuestOrder> OrderType)
{
	if (OrderType == nullptr)
	{
		return EQuestOrderTargetType::NONE;
	}

	if (!OrderType.IsValid())
	{
		OrderType.LoadSynchronous();
	}

	return OrderType->GetDefaultObject<UQuestOrder>()->GetTargetType();
}

EQuestOrderCancellationPolicy UQuestOrderHelperLibrary::GetCancellationPolicy(TSoftClassPtr<UQuestOrder> OrderType)
{
	if (OrderType == nullptr)
	{
		return EQuestOrderCancellationPolicy::CAN_BE_CANCELLED;
	}

	if (!OrderType.IsValid())
	{
		OrderType.LoadSynchronous();
	}

	return OrderType->GetDefaultObject<UQuestOrder>()->GetCancellationPolicy();
}

float UQuestOrderHelperLibrary::GetRange(TSoftClassPtr<UQuestOrder> OrderType)
{
	if (!OrderType.IsValid())
	{
		OrderType.LoadSynchronous();
	}
	return OrderType->GetDefaultObject<UQuestOrder>()->GetRange();
}

UBehaviorTree* UQuestOrderHelperLibrary::GetBehaviorTree(TSoftClassPtr<UQuestOrder> OrderType)
{
	if (OrderType == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestOrderHelperLibrary::GetBehaviorTree: no valid order type!"))
			return nullptr;
	}
	if (!OrderType.IsValid())
	{
		OrderType.LoadSynchronous();
	}
	return OrderType->GetDefaultObject<UQuestOrder>()->GetBehaviorTree();
}

bool UQuestOrderHelperLibrary::ShouldRestartBehaviorTree(TSoftClassPtr<UQuestOrder> OrderType)
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
