// Copyright 2019 Kelby Van Patten


#include "QuestCharacterBase.h"
#include "QuestAttributeSet.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "Components/SphereComponent.h"

// Sets default values
AQuestCharacterBase::AQuestCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create ability and attribute components
	AbilitySystemComponent = CreateDefaultSubobject<UQuestAbilitySystemComponent>("AbilitySystemComponent");
	AttributeSetComponent = CreateDefaultSubobject<UQuestAttributeSet>("AttributeSet");

	//  Acquire basic abilities

	// Create InteractionSphere that tells us whether character is close enough to attack or interact with another character
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("MeleeAttackSphere"));
	InteractionSphereRadius = 130.f;
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->SetSphereRadius(InteractionSphereRadius);

	bIsDead = false;
	bIsTargetWithinInteractionSphere = false;
	TargetActor = nullptr;
}

// Called when the game starts or when spawned
void AQuestCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	/** Subscribe to the OnHealthChange broadcast from our Attribute Set, and when we receive it, run OnHealthChanged */
	AttributeSetComponent->OnHealthChange.AddDynamic(this, &AQuestCharacterBase::OnHealthChanged);

	/** Subscribe to the Begin and End Overlap events broadcast from the Interaction Sphere */
	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AQuestCharacterBase::OnInteractionSphereBeginOverlap);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AQuestCharacterBase::OnInteractonSphereEndOverlap);

	/** Subscribe to the QuestGameMode delegate that tells us when we are entering or exiting combat mode */
	GameMode = Cast<AQuestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->OnCombatModeChange.AddDynamic(this, &AQuestCharacterBase::OnCombatModeChanged);

}

// Called every frame
void AQuestCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AQuestCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Gives this character an ability system component
UAbilitySystemComponent* AQuestCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Gives this character an ability
void AQuestCharacterBase::AcquireAbility(TSubclassOf<UGameplayAbility>AbilityToAcquire)
{
	FString Name = AbilityToAcquire->GetName();
	if (AbilitySystemComponent)
	{
		if (HasAuthority() && AbilityToAcquire)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToAcquire));
		}
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void AQuestCharacterBase::AddGameplayTag(FGameplayTag TagToAdd)
{
	GetAbilitySystemComponent()->AddLooseGameplayTag(TagToAdd);
	GetAbilitySystemComponent()->SetTagMapCount(TagToAdd, 1);
}

void AQuestCharacterBase::RemoveGameplayTag(FGameplayTag TagToRemove)
{
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(TagToRemove);
}

/** Called when the AttributeSet broadcasts an OnHealthChanged delegate; */

void AQuestCharacterBase::OnHealthChanged(float Health, float MaxHealth)
{ 
	// Check to see whether the character is dead
	if (Health <= 0)
	{
		bIsDead = true;
		BP_Die();
	}

	// call the Blueprint function that determines what happens when health changed
	UE_LOG(LogTemp, Warning, TEXT("Health down to %f"), AttributeSetComponent->Health.GetCurrentValue());
	BP_OnHealthChanged(Health, MaxHealth);
}

void AQuestCharacterBase::OnInteractionSphereBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (TargetActor && TargetActor == OtherActor)
	{
		bIsTargetWithinInteractionSphere = true;
	}
}

void AQuestCharacterBase::OnInteractonSphereEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (TargetActor && TargetActor == OtherActor)
	{
		bIsTargetWithinInteractionSphere = false;
	}
}

void AQuestCharacterBase::OnCombatModeChanged(bool NewbIsCombatModeActive)
{
		bIsCombatModeActive = NewbIsCombatModeActive;
		UE_LOG(LogTemp, Warning, TEXT("bIsCombatModeActive changed to %s"), *FString(bIsCombatModeActive ? "true" : "false"))
}

void AQuestCharacterBase::MeleeAttack()
{
	BP_MeleeAttack();
}

void AQuestCharacterBase::SetTargetActorToNull()
{
	if (TargetActor)
	{
		TargetActor = nullptr;
	}
}



