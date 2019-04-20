// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestCharacterBase.h"
#include "QuestAttributeSet.h"
#include "GameFramework/Actor.h"

// Sets default values
AQuestCharacterBase::AQuestCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UQuestAbilitySystemComponent>("AbilitySystemComponent");
	AttributeSetComponent = CreateDefaultSubobject<UQuestAttributeSet>("AttributeSet");
}

// Called when the game starts or when spawned
void AQuestCharacterBase::BeginPlay()
{
	Super::BeginPlay();
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

// Gets the ability system component
UAbilitySystemComponent* AQuestCharacterBase::GetAbilitySystemComponent() const
{
	
	return AbilitySystemComponent;
}

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
//

