// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestGATargetActorClicked.h"
#include "GameplayAbility.h"
#include "Kismet/GameplayStatics.h"
#include "QuestAIController.h"
#include "QuestPlayerController.h"
#include "QuestCharacterBase.h"

AQuestGATargetActorClicked::AQuestGATargetActorClicked()
{

}

void AQuestGATargetActorClicked::StartTargeting(UGameplayAbility* Ability)
{

	// Override ability and controller variables from parent class
	OwningAbility = Ability;

	MasterPC = Cast<AQuestPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (IsValid(MasterPC))

	{
		// Keep the mouse from moving the character while targeting is active
		UE_LOG(LogTemp, Warning, TEXT("QuestGATargetActorClicked:StartTargeting:  MasterPC is valid!"))
		PlayerController = Cast<AQuestPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (IsValid(PlayerController))
		{

			PlayerController->bControllerShouldDirectCharacter = false;
			PlayerController->bIsTargeting = true;
			PlayerController->CurrentMouseCursor = EMouseCursor::Hand;
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("QuestGATargetActorClicked:StartTargeting:  Player controller is not valid!"))
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestGATargetActorClicked:StartTargeting:  MasterPC is not valid!"))
	}
}

void AQuestGATargetActorClicked::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	PlayerController->bControllerShouldDirectCharacter = true;
	PlayerController->bIsTargeting = false;
	PlayerController->CurrentMouseCursor = EMouseCursor::Default;
}

void AQuestGATargetActorClicked::ConfirmTargetingAndContinue()
{
	UE_LOG(LogTemp, Warning, TEXT("QuestGATargetActorClicked::ConfirmTargetingingAndContinue:  Confirming!"));
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
