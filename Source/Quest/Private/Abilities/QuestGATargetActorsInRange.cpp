// Copyright 2019 Kelby Van Patten


#include "QuestGATargetActorsInRange.h"
#include "GameFramework/PlayerController.h"
#include "GameplayAbility.h"
#include "Kismet/GameplayStatics.h"
#include "QuestCharacterBase.h"
#include "QuestPlayerController.h"

AQuestGATargetActorsInRange::AQuestGATargetActorsInRange()
{
}

void AQuestGATargetActorsInRange::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	MasterPC = Cast<AQuestPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void AQuestGATargetActorsInRange::ConfirmTargetingAndContinue()
{
	AQuestPlayerController* PlayerController = Cast<AQuestPlayerController>(MasterPC);
	// Create an array of actors within the target radius of the location set by the ability (usually the character's location)
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
				UE_LOG(LogTemp, Warning, TEXT("QuestGATargetActorsInRange::ConfirmTargetingAndContinue: adding %s to target list!"),
					*Character->GetName());
			}
		}
	}
	// Put the array and cursor location in a Data Handle and broadcast the Data Handle

	if (CharactersInRange.Num() > 0)

	{
		FGameplayAbilityTargetDataHandle CharactersInRageData = StartLocation.MakeTargetDataHandleFromActors(CharactersInRange);
		TargetDataReadyDelegate.Broadcast(CharactersInRageData);
	}
	else
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
	}

		// Enable character movement on mouse click
	if (IsValid(PlayerController))
	{
		PlayerController->bControllerShouldDirectCharacter = true;
		PlayerController->bIsTargeting = false;
	}
}
