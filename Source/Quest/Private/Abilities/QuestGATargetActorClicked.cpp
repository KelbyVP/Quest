// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestGATargetActorClicked.h"
#include "GameplayAbility.h"
#include "QuestPlayerController.h"
#include "QuestCharacterBase.h"

AQuestGATargetActorClicked::AQuestGATargetActorClicked()
{

}

void AQuestGATargetActorClicked::StartTargeting(UGameplayAbility* Ability)
{
	// Override ability and controller variables from parent class
	OwningAbility = Ability;
	MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());

	// Keep the mouse from moving the character while targeting is active
	PlayerController = Cast<AQuestPlayerController>(MasterPC);
	PlayerController->bControllerShouldMoveCharacter = false;
	PlayerController->bIsTargeting = true;
	PlayerController->CurrentMouseCursor = EMouseCursor::Hand;
}

void AQuestGATargetActorClicked::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	PlayerController->bControllerShouldMoveCharacter = true;
	PlayerController->CurrentMouseCursor = EMouseCursor::Default;
}

void AQuestGATargetActorClicked::ConfirmTargetingAndContinue()
{
	FHitResult Hit;
	if (MasterPC != nullptr)
	{
		MasterPC->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
		if (Hit.bBlockingHit)
		{
			AActor* ActorClicked = Hit.GetActor();
			AQuestCharacterBase* PawnClicked = Cast<AQuestCharacterBase>(ActorClicked);
			if (PawnClicked)
			{
				TArray<TWeakObjectPtr<AActor>> TargetActorArray;
				TargetActorArray.Add(ActorClicked);
				FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(TargetActorArray);
				TargetDataReadyDelegate.Broadcast(TargetData);
				return;
			}
		}
	}
	TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
}
