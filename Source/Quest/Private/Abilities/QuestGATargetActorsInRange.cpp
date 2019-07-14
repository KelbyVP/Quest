// Copyright 2019 Kelby Van Patten


#include "QuestGATargetActorsInRange.h"
#include "GameFramework/PlayerController.h"
#include "QuestCharacterBase.h"
#include "GameplayAbility.h"


AQuestGATargetActorsInRange::AQuestGATargetActorsInRange()
{
}

void AQuestGATargetActorsInRange::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
}

void AQuestGATargetActorsInRange::ConfirmTargetingAndContinue()
{

	// Create an array of actors within the target radius of the cursor location
	TArray<TWeakObjectPtr<AActor>> CharactersInRange;
	TArray<FOverlapResult> Overlaps;
	bool TraceComplex = false;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bTraceComplex = TraceComplex;
	CollisionQueryParams.bReturnPhysicalMaterial = false;
	bool TryOverlap = GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		TargetLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECC_Pawn),
		FCollisionShape::MakeSphere(TargetRadius),
		CollisionQueryParams);
	if (TryOverlap)
	{
		for (const auto& o : Overlaps)
		{
			if (auto Character = Cast<APawn>(o.GetActor()))
			{
				CharactersInRange.AddUnique(Character);
			}
		}
	}

	if (CharactersInRange.Num() > 0)

	{
		FGameplayAbilityTargetDataHandle CharactersInRageData = StartLocation.MakeTargetDataHandleFromActors(CharactersInRange);
		TargetDataReadyDelegate.Broadcast(CharactersInRageData);
	}
	else
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
	}
}
