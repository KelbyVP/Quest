// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "QuestCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "CollisionQueryParams.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Math/Vector.h"
#include "Kismet/GameplayStatics.h"
#include "QuestAttributeSet.h"
#include "QuestAutoOrderComponent.h"
#include "QuestMerchantCharacter.h"
#include "QuestOrderHandlingComponent.h"
#include "QuestPlayerController.h"
#include "QuestSelectableComponent.h"
#include "QuestStorage.h"
#include "UObject/ConstructorHelpers.h"

AQuestCharacter::AQuestCharacter()
{


	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//  TODO:  Marked for possible deletion
	//// Don't rotate character to camera direction
	//bUseControllerRotationPitch = false;
	//bUseControllerRotationYaw = false;
	//bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a selectable component...
	SelectableComponent = CreateDefaultSubobject<UQuestSelectableComponent>("SelectableComponent");

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bIsHostile = false;
	Affiliation = ECharacterAffiliation::IT_Friendly;
	StorageChest = nullptr;
	Experience = 500000;
}

void AQuestCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AQuestCharacter::LeaveCombat()
{
	if (OrderHandlingComponent->bIsBeingDirectedByPlayer)
	{
		return;
	}
	AutoOrderComponent->GenerateAutoOrder();
}

void AQuestCharacter::CastSpell(TSubclassOf<UQuestGameplayAbility> Ability)
{
	UE_LOG(LogTemp, Warning, TEXT
	("QuestCharacter::CastSpell: Casting quest character class spell with ability %s"), *Ability->GetName());
	OrderHandlingComponent->IssuePlayerDirectedOrderWithAbility(Ability);
}

void AQuestCharacter::InitializeCharacterGroup()
{

}

void AQuestCharacter::OnLeaveStorage()
{
	BP_OnLeaveStorage();
}

void AQuestCharacter::OnLeaveMerchant()
{
	BP_OnLeaveMerchant();
}

void AQuestCharacter::TryLevelUp()
{
	if (Experience >= NextLevelExperience)
	{
		AttributeSetComponent->LevelUp();
		BP_OnLevelUp();
	}
}
