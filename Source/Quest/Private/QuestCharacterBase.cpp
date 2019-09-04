// Copyright 2019 Kelby Van Patten


#include "QuestCharacterBase.h"
#include "CollisionQueryParams.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameModeBase.h"
#include "GameplayTagContainer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "QuestAttributeSet.h"
#include "QuestAutoOrderComponent.h"
#include "QuestCharacterGroup.h"
#include "QuestGlobalTags.h"
#include "QuestOrderHandlingComponent.h"
#include "QuestOrderHelperLibrary.h"
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
	bIsLeader = false;
	CharacterGroup = nullptr;
	GroupRange = 2000.0f;

	// Variables for the AI Perception Component
	AISightRadius = 1500.0f;
	AISightAge = 5.0f;
	AILoseSightRadius = 550.0f;
	AIFieldOfView = 180.0f;

	Affiliation = ECharacterAffiliation::IT_Neutral;

	// Add the default gameplay tags to this character
}

// Called when the game starts or when spawned
void AQuestCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	/** Subscribe to the OnHealthChange broadcast from our Attribute Set, and when we receive it, run OnHealthChanged */
	AttributeSetComponent->OnHealthChange.AddDynamic(this, &AQuestCharacterBase::OnHealthChanged);

	/** Add default tags */
	if (DefaultTags.Num() > 0)
	{
		AddGameplayTags(DefaultTags);
	}

	/** Initialize character group */
	if (bIsLeader)
	{
		if (CharacterGroup == nullptr)		
		{
			InitializeCharacterGroup();
			AddMembersToCharacterGroup();
		}
	}
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
	if (UQuestGlobalTags::CanTagBeStacked(TagToAdd))
	{
		GetAbilitySystemComponent()->SetTagMapCount(TagToAdd, 1);
	}
}

void AQuestCharacterBase::AddGameplayTags(TArray<FGameplayTag> TagsToAdd)
{
	for (auto Tag : TagsToAdd)
	{
		AddGameplayTag(Tag);
	}
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
		// TODO:  If the character is a leader and dies, we need to see whether there are any other characters in the character group, and if so, make one of them the leader
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

void AQuestCharacterBase::SetCharacterGroup(AQuestCharacterGroup* InCharacterGroup)
{
	CharacterGroup = InCharacterGroup;
	CharacterGroup->OnEnterCombat.AddDynamic(this, &AQuestCharacterBase::EnterCombat);
}

void AQuestCharacterBase::CreateCharacterGroup()
{
	/** Try to find a leader without a CharacterGroup and ask that leader to form a CharacterGroup */
	if (!CharacterGroup && !bIsLeader)
	{
		TArray<AQuestCharacterBase*> CharactersInRange;
		CharactersInRange = UQuestOrderHelperLibrary::GetCharactersInRange(this, GroupRange);
		for (auto& Character : CharactersInRange)
		{
			if (!Character->CharacterGroup && Character->bIsLeader)
			{
				Character->InitializeCharacterGroup();
				Character->AddMembersToCharacterGroup();
				break;
			}
		}
	}
	/** If we still don't have a CharacterGroup, become a leader and create one */
	if (!CharacterGroup)
	{
		bIsLeader = true;
		InitializeCharacterGroup();
		AddMembersToCharacterGroup();
	}
}

void AQuestCharacterBase::EnterCombat()
{
	if (AutoOrderComponent)
	{
		/** if we are already in combat, do nothing */
		if (AutoOrderComponent->bIsInCombat)
		{
			return;
		}

		/** If we are not already in combat, begin */
		AutoOrderComponent->EnterCombat();
	}
}

bool AQuestCharacterBase::IsAdverse(const AQuestCharacterBase* OtherActor)
{
	if (OtherActor->Affiliation != ECharacterAffiliation::IT_Neutral
		&& OtherActor->Affiliation != Affiliation)
	{
		return true ;
	}
	else { return false; }
}

void AQuestCharacterBase::SetAutoOrderAsUsed(TSoftClassPtr<UQuestOrder> Order)
{
	int32 Index;
	FQuestAutoOrder AutoOrder = FQuestAutoOrder(Order);
	if (AutoOrder.FindAutoOrderInArray(AutoOrderArray, Index))
	{
		AutoOrderArray[Index].bHasBeenUsed = true;
	}
}

void AQuestCharacterBase::InitializeCharacterGroup()
{
	CharacterGroup = GetWorld()->SpawnActor<AQuestCharacterGroup>(AQuestCharacterGroup::StaticClass(), FVector(0, 0, 0), FRotator(), FActorSpawnParameters());
	CharacterGroup->AddCharacter(this);
	CharacterGroup->SetLeader(this);
}

void AQuestCharacterBase::AddMembersToCharacterGroup()
{
	/** Set variables for sweep */
	TArray<FHitResult> CharactersInRange;
	FVector Start = GetActorLocation();
	FVector End = Start + FVector(0, 0, 1);
	float SweepSphereRadius = GroupRange;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredComponent(GetCapsuleComponent());

	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

	/** Run sweep */
	GetWorld()->SweepMultiByObjectType(
		CharactersInRange,
		Start,
		End,
		FQuat::Identity,
		ObjParams,
		FCollisionShape::MakeSphere(SweepSphereRadius),
		QueryParams
	);

	/** Add found characters with same affiliation to the character group if they are not already leaders */
	if (CharactersInRange.Num() > 0)
	{
		for (auto& Hit : CharactersInRange)
		{
			AQuestCharacterBase* PossibleMember = Cast<AQuestCharacterBase>(Hit.GetActor());
			if (PossibleMember && 
				PossibleMember->CharacterGroup == nullptr && 
				Affiliation == PossibleMember->Affiliation &&
				!PossibleMember->bIsLeader
				)
			{
				CharacterGroup->AddCharacter(PossibleMember);
			}
		}
	}
}

bool AQuestCharacterBase::DoesCharacterHaveTag(FGameplayTag const& Tag)
{
	FGameplayTagContainer TagContainer;
	// TODO:  Can delete?
	//GetOwnedGameplayTags(TagContainer);
	AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
	if (TagContainer.HasTag(Tag))
	{
		return true;
	}
	return false;
}

