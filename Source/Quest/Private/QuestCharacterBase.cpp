// Copyright 2019 Kelby Van Patten


#include "QuestCharacterBase.h"
#include "QuestAttributeSet.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "Components/SphereComponent.h"
#include "QuestAutoOrderComponent.h"
#include "QuestOrderHandlingComponent.h"
#include "QuestSpellbook.h"
#include "QuestSpells.h"

// Sets default values
AQuestCharacterBase::AQuestCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create ability and attribute components
	AbilitySystemComponent = CreateDefaultSubobject<UQuestAbilitySystemComponent>("AbilitySystemComponent");
	AttributeSetComponent = CreateDefaultSubobject<UQuestAttributeSet>("AttributeSet");

	// Create order system components
	OrderHandlingComponent = CreateDefaultSubobject<UQuestOrderHandlingComponent>("OrderHandlingComponent");
	AutoOrderComponent = CreateDefaultSubobject<UQuestAutoOrderComponent>("AutoOrderComponent");

	bIsDead = false;
	CharacterClass = ECharacterClass::IT_Wizard;
}

// Called when the game starts or when spawned
void AQuestCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	/** Subscribe to the OnHealthChange broadcast from our Attribute Set, and when we receive it, run OnHealthChanged */
	AttributeSetComponent->OnHealthChange.AddDynamic(this, &AQuestCharacterBase::OnHealthChanged);
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

TSoftClassPtr<UQuestDefaultOrder> AQuestCharacterBase::GetDefaultOrder() const
{
	return DefaultOrder;
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

void AQuestCharacterBase::MeleeAttack()
{
	BP_MeleeAttack();
}

void AQuestCharacterBase::SetSpellbookType()
{
	if (Spellbook)
	{
		if (CharacterClass == ECharacterClass::IT_Paladin)
		{
			Spellbook->SpellbookType = ESpellType::IT_Paladin;
		}
		else if (CharacterClass == ECharacterClass::IT_Priest)
		{
			Spellbook->SpellbookType = ESpellType::IT_Priest;
		}
		else if (CharacterClass == ECharacterClass::IT_Wizard)
		{
			Spellbook->SpellbookType = ESpellType::IT_Wizard;
		}
		else
		{
			Spellbook->SpellbookType = ESpellType::IT_Other;
		}
	}
}

bool AQuestCharacterBase::DoesCharacterHaveTag(FGameplayTag const& Tag)
{
	FGameplayTagContainer TagContainer;
	GetOwnedGameplayTags(TagContainer);
	AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
	if (TagContainer.HasTag(Tag))
	{
		return true;
	}
	return false;
}

