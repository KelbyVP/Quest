// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestOrderHelperLibrary.h"
#include "AbilitySystemComponent.h"
#include "CollisionQueryParams.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Math/UnrealMathUtility.h"
#include "Math/Vector.h"
#include "QuestAbilitySystemHelper.h"
#include "QuestAttributeSet.h"
#include "QuestCharacterBase.h"
#include "QuestCharacterGroup.h"
#include "QuestOrder.h"
#include "QuestOrderErrorTags.h"
#include "QuestOrderTargetData.h"
#include "QuestOrderTargetType.h"


/** 
*	CanObeyOrder tells whether the ordered character has the right tags 
*/

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

FQuestOrderTargetData UQuestOrderHelperLibrary::CreateTargetDataForOrder(const AActor* OrderedActor, AActor* TargetActor, const FVector& TargetLocation)
{
	FQuestOrderTargetData TargetData;
	TargetData.Actor = TargetActor;
	TargetData.Location = TargetLocation;

	/** If there is no target actor, just return the target data with location only */
	if (TargetActor == nullptr)
	{
		return TargetData;
	}

	/** If we have a target actor, also create the target tags */

	FGameplayTagContainer SourceTags;
	FGameplayTagContainer TargetTags;
	UQuestAbilitySystemHelper::GetSourceAndTargetTags(OrderedActor, TargetActor, SourceTags, TargetTags);

	TargetData.TargetTags = TargetTags;
	return TargetData;
}

EQuestOrderTargetScoringMethod UQuestOrderHelperLibrary::GetTargetScoringMethod(TSoftClassPtr<UQuestOrder> OrderType)
{
	if (OrderType == nullptr)
	{
		return EQuestOrderTargetScoringMethod::CHOOSE_CLOSEST_ADVERSARY_TARGET;
	}

	if (!OrderType.IsValid())
	{
		OrderType.LoadSynchronous();
	}

	return OrderType->GetDefaultObject<UQuestOrder>()->GetTargetScoringMethod();
}

float UQuestOrderHelperLibrary::GetTargetAcquisitionRange(TSoftClassPtr<UQuestOrder> OrderType)
{
	if (OrderType == nullptr)
	{
		return 0.0f;
	}

	if (!OrderType.IsValid())
	{
		OrderType.LoadSynchronous();
	}

	return OrderType->GetDefaultObject<UQuestOrder>()->GetTargetAcquisitionRange();
}

TArray<AQuestCharacterBase*> UQuestOrderHelperLibrary::GetHostileTargetsInRange(const AQuestCharacterBase* OrderedCharacter, float Radius)
{
	TArray<AQuestCharacterBase*> PossibleTargetsInRange;
	TArray<AQuestCharacterBase*> HostileTargetsInRange;
	PossibleTargetsInRange = GetCharactersInRange(OrderedCharacter, Radius);

	/** Filter targets with whom we are in combat */
	if (PossibleTargetsInRange.Num() > 0)
	{
		for (auto& PossibleTarget : PossibleTargetsInRange)
		{
			if (IsValid(PossibleTarget))
			{

				if (PossibleTarget->IsAdverse(OrderedCharacter))
				{
					HostileTargetsInRange.Add(PossibleTarget);
					if (IsValid(OrderedCharacter) && IsValid(OrderedCharacter->CharacterGroup))
					{
						OrderedCharacter->CharacterGroup->CheckShouldStartFighting(PossibleTarget);
					}
				}
			}
		}
	}
	return HostileTargetsInRange;
}

AQuestCharacterBase* UQuestOrderHelperLibrary::SelectClosestTarget(const AActor* OrderedCharacter, TArray<AQuestCharacterBase*> TargetCharacters)
{
	AQuestCharacterBase* ClosestTarget = nullptr;
	float DistanceToTarget = 999999.f;

	if (TargetCharacters.Num() > 0)
	{
		ClosestTarget = TargetCharacters[0];
		DistanceToTarget = FVector::Dist(OrderedCharacter->GetActorLocation(), ClosestTarget->GetActorLocation());
		for (auto& Character : TargetCharacters)
		{
			float DistanceToCharacter = FVector::Dist(OrderedCharacter->GetActorLocation(), Character->GetActorLocation());
			if (DistanceToCharacter < DistanceToTarget)
			{
				ClosestTarget = Character;
				DistanceToTarget = DistanceToCharacter;
			}
		}
	}
	return ClosestTarget;
}

AQuestCharacterBase* UQuestOrderHelperLibrary::SelectClosestHostileTarget(const AQuestCharacterBase* OrderedCharacter, TSoftClassPtr<UQuestOrder> OrderType)
{
	float TargetAcquisitionRange = GetTargetAcquisitionRange(OrderType);
	TArray<AQuestCharacterBase*> TargetsInRange = GetHostileTargetsInRange(OrderedCharacter, TargetAcquisitionRange);
	return SelectClosestTarget(OrderedCharacter, TargetsInRange);
}

AQuestCharacterBase* UQuestOrderHelperLibrary::SelectClosestHostileLeaderInAcquisitionRange(const AQuestCharacterBase* OrderedCharacter, TSoftClassPtr<UQuestOrder> OrderType)
{
	float Range = GetTargetAcquisitionRange(OrderType);
	float Distance = Range +1;
	AQuestCharacterBase* ClosestLeader = nullptr;
	FVector OrderedCharacterLocation = OrderedCharacter->GetActorLocation();
	for (auto& AdverseGroup : OrderedCharacter->CharacterGroup->AdverseGroupsInCombat)
	{
		float LeaderDistance = FVector::Dist(AdverseGroup->Leader->GetActorLocation(), OrderedCharacterLocation);
		if (LeaderDistance < Distance &&
			LeaderDistance < Range)
		{
			Distance = LeaderDistance;
			ClosestLeader = AdverseGroup->Leader;
		}
	}

	return ClosestLeader;
}

AQuestCharacterBase* UQuestOrderHelperLibrary::SelectMostPowerfulAdversaryInAcquisitionRange(const AQuestCharacterBase* OrderedCharacter, TSoftClassPtr<UQuestOrder> OrderType)
{
	float AcquisitionRange = GetTargetAcquisitionRange(OrderType);
	TArray<AQuestCharacterBase*> HostileTargetsInRange = GetHostileTargetsInRange(OrderedCharacter, AcquisitionRange);
	return GetMostPowerfulCharacterInArray(HostileTargetsInRange);
}

AQuestCharacterBase* UQuestOrderHelperLibrary::SelectMostPowerfulAdverseSpellcasterInAcquisitionRange(const AQuestCharacterBase* OrderedCharacter, TSoftClassPtr<UQuestOrder> OrderType)
{
	AQuestCharacterBase* MostPowerfulSpellcaster = nullptr;
	float AcquisitionRange = GetTargetAcquisitionRange(OrderType);
	TArray<AQuestCharacterBase*> HostileTargetsInRange = GetHostileTargetsInRange(OrderedCharacter, AcquisitionRange);
	TArray<AQuestCharacterBase*> SpellcastersInRange;
	if (HostileTargetsInRange.Num() > 0)
	{
		for (auto& Target : HostileTargetsInRange)
		{
			if (Target->bIsSpellcaster)
			{
				SpellcastersInRange.AddUnique(Target);
			}
		}
	}
	if (SpellcastersInRange.Num() > 0)
	{
		MostPowerfulSpellcaster = GetMostPowerfulCharacterInArray(SpellcastersInRange);
		if (!MostPowerfulSpellcaster)
		{
			MostPowerfulSpellcaster = SelectMostPowerfulAdversaryInAcquisitionRange(OrderedCharacter, OrderType);
		}
		if (!MostPowerfulSpellcaster)
		{
			MostPowerfulSpellcaster = SelectClosestHostileTarget(OrderedCharacter, OrderType);
		}
	}
	return MostPowerfulSpellcaster;
}

AQuestCharacterBase* UQuestOrderHelperLibrary::SelectMostPowerfulAlliedLeaderInAcquisitionRange(const AQuestCharacterBase* OrderedCharacter, TSoftClassPtr<UQuestOrder> OrderType)
{
	TArray<AQuestCharacterBase*> Characters;
	float Radius = GetTargetAcquisitionRange(OrderType);
	Characters = GetCharactersInRange(OrderedCharacter, Radius);
	TArray<AQuestCharacterBase*> AlliedLeaders;

	/** Remove characters that are not leaders or allies */
	for (auto& Character : Characters)
	{
		if (Character->bIsLeader &&
			Character->Affiliation == OrderedCharacter->Affiliation
			)
		{
			AlliedLeaders.AddUnique(Character);
		}
	}

	/** If we have ally leaders in range, return the most powerful one */
	if (AlliedLeaders.Num() > 0)
	{
		return GetMostPowerfulCharacterInArray(AlliedLeaders);
	}

	/** If we have no ally leaders in range, return most powerful ally in range */
	else
	{
		TArray<AQuestCharacterBase*> Allies;
		for (auto& Character: Characters)
		{
			if (Character->Affiliation == OrderedCharacter->Affiliation)
			{
				Allies.AddUnique(Character);
			}
		}
		return GetMostPowerfulCharacterInArray(Allies);
	}
}

AQuestCharacterBase* UQuestOrderHelperLibrary::GetMostPowerfulCharacterInArray(TArray<AQuestCharacterBase*>& CharacterArray)
{
	AQuestCharacterBase* MostPowerfulAdversary = nullptr;
	float MostPowerfulLevel = 0.f;
	float MostPowerfulHealth = 0.f;

	if (CharacterArray.Num() > 0)
	{
		for (auto& Target : CharacterArray)
		{
			float TargetLevel = Target->AttributeSetComponent->Level.GetCurrentValue();
			if (FMath::IsNearlyEqual(TargetLevel, MostPowerfulLevel))
			{
				float TargetHealth = Target->AttributeSetComponent->Health.GetCurrentValue();
				if (TargetHealth > MostPowerfulHealth)
				{
					MostPowerfulAdversary = Target;
					MostPowerfulHealth = TargetHealth;
					MostPowerfulLevel = TargetLevel;
				}
			}
			else if (TargetLevel > MostPowerfulLevel)
			{
				MostPowerfulAdversary = Target;
				MostPowerfulHealth = Target->AttributeSetComponent->Health.GetCurrentValue();
				MostPowerfulLevel = TargetLevel;
			}
		}
	}
	return MostPowerfulAdversary;
}

TArray<AQuestCharacterBase*> UQuestOrderHelperLibrary::GetCharactersInRange(const AQuestCharacterBase* OrderedCharacter, float Radius)
{
	/** Set variables for sweep */
	TArray<FHitResult> Hits;
	FVector OrderedCharacterLocation = OrderedCharacter->GetActorLocation();
	FVector End = OrderedCharacterLocation + FVector(0, 0, 1);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OrderedCharacter);
	QueryParams.AddIgnoredComponent(OrderedCharacter->GetCapsuleComponent());

	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

	/** Run sweep */
	OrderedCharacter->GetWorld()->SweepMultiByObjectType(
		Hits,
		OrderedCharacterLocation,
		End,
		FQuat::Identity,
		ObjParams,
		FCollisionShape::MakeSphere(Radius),
		QueryParams
	);

	/** Filter unique QuestCharacterBase actors */
	TArray<AQuestCharacterBase*> CharactersInRange;

	for (auto& Hit : Hits)
	{
		AQuestCharacterBase* Character = Cast<AQuestCharacterBase>(Hit.GetActor());
		if (Character)
		{
			CharactersInRange.AddUnique(Character);
		}
	}

	return CharactersInRange;
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

AQuestCharacterBase* UQuestOrderHelperLibrary::SelectTarget(const AQuestCharacterBase* OrderedCharacter, TSoftClassPtr<UQuestOrder> OrderType)
{
	AQuestCharacterBase* TargetCharacter = nullptr;
	EQuestOrderTargetScoringMethod TargetScoringMethod = GetTargetScoringMethod(OrderType);

	// TODO:  Delete FStrings; these are only used for the UE Logs
	FString TargetName;
	FString Name = OrderType->GetName();
	
	switch (TargetScoringMethod)
	{
	case EQuestOrderTargetScoringMethod::CHOOSE_CLOSEST_ADVERSARY_TARGET:
		TargetCharacter = SelectClosestHostileTarget(OrderedCharacter, OrderType);
		break;

	case EQuestOrderTargetScoringMethod::CHOOSE_ADVERSARY_LEADER:
		TargetCharacter = SelectClosestHostileLeaderInAcquisitionRange(OrderedCharacter, OrderType);

		if (TargetCharacter == nullptr)
		{
			TargetCharacter = SelectClosestHostileTarget(OrderedCharacter, OrderType);
			break;
		}
		break;

	case EQuestOrderTargetScoringMethod::CHOOSE_MOST_POWERFUL_ADVERSARY_TARGET:
		TargetCharacter = SelectMostPowerfulAdversaryInAcquisitionRange(OrderedCharacter, OrderType);
		break;

	case EQuestOrderTargetScoringMethod::CHOOSE_ADVERSARY_SPELLCASTER:
		TargetCharacter = SelectMostPowerfulAdverseSpellcasterInAcquisitionRange(OrderedCharacter, OrderType);
		break;

	case EQuestOrderTargetScoringMethod::CHOOSE_ALLIED_LEADER:
		TargetCharacter = SelectMostPowerfulAlliedLeaderInAcquisitionRange(OrderedCharacter, OrderType);
		break;
	default:
		break;
	}
	return TargetCharacter;
}

bool UQuestOrderHelperLibrary::IsValidTarget(TSoftClassPtr<UQuestOrder> OrderType, const AActor* OrderedActor, const FQuestOrderTargetData& TargetData)
{
	if (OrderType == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestOrderHelperLibrary::IsValidTarget: %s had a null pointer for an order!"), *OrderedActor->GetName());
		return false;
	}

	if (!OrderType.IsValid())
	{
		OrderType.LoadSynchronous();
	}

	/** Create an instance of the order so we can have it check the target's validity */
	const UQuestOrder* Order = OrderType->GetDefaultObject<UQuestOrder>();

	/** Check to see whether the target actor is valid and meets the tag requirements */
	EQuestOrderTargetType TargetType = Order->GetTargetType();
	if (TargetType == EQuestOrderTargetType::ACTOR)
	{
		if (!IsValid(TargetData.Actor))
		{
			UE_LOG(LogTemp, Warning, TEXT("QuestOrderHelperLibrary::IsValidTarget: %s had a null pointer for a target!"), *OrderedActor->GetName());
			return false;
		}
		FQuestOrderTagRequirements TagRequirements;
		Order->GetTagRequirements(OrderedActor, TagRequirements);
		if (!UQuestAbilitySystemHelper::DoesSatisfyTagRequirements(TargetData.TargetTags, TagRequirements.TargetTagsRequired, TagRequirements.TargetTagsBlocked))
		{
			UE_LOG(LogTemp, Warning, TEXT("QuestOrderHelperLibrary::IsValidTarget: %s did not satisfy tag requirements!"), *OrderedActor->GetName());
			return false;
		}
	}

	/** Check to see whether the target meets any additional requirements the order may have */
	return Order->IsValidTarget(OrderedActor, TargetData);
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
