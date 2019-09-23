// Copyright 2019 Kelby Van Patten

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GameplayAbility.h"
#include "GameplayTagAssetInterface.h"
#include "QuestAbilitySystemComponent.h"
#include "QuestAutoOrder.h"
#include "QuestWeaponItem.h"
#include "QuestShieldItem.h"
#include "QuestGameMode.h"
#include "QuestOrderData.h"
#include "QuestCharacterBase.generated.h"

class AQuestCharacterGroup;
class AQuestSpellbook;
class AQuestCharacterRotationActor;
class UAnimMontage;
class UQuestAttributeSet;
class UQuestAutoOrderComponent;
class UQuestDefaultOrder;
class UQuestDefaultAbilities;
class UQuestGameplayAbility;
class UQuestOrder;
class UQuestOrderHandlingComponent;
class UQuestRotationComponent;
class UQuestUseAbilityOrder;
class USphereComponent;


UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	IT_Fighter UMETA(DisplayName = "Fighter"),
	IT_Paladin UMETA(DisplayName = "Paladin"),
	IT_Ranger UMETA(DisplayName = "Ranger"),
	IT_Thief UMETA(DisplayName = "Thief"),
	IT_Wizard UMETA(DisplayName = "Wizard"),
	IT_Priest UMETA(DisplayName = "Priest")
};

/** Describes the character's affiliation to the player's party */
UENUM(BlueprintType)
enum class ECharacterAffiliation : uint8
{
	IT_Hostile UMETA(DisplayName = "Hostile"),
	IT_Neutral UMETA(DisplayName = "Neutral"),
	IT_Friendly UMETA(DisplayName = "Friendly")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS()
class QUEST_API AQuestCharacterBase : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AQuestCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//  Give the character default abilities
	void AcquireDefaultAbilities();

public:	
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	UFUNCTION()
		TSoftClassPtr<UQuestDefaultOrder> GetDefaultOrder() const;

	/** Basic functions to implement the ability system */
	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void AcquireAbility(TSubclassOf<UGameplayAbility>AbilityToAcquire);
	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void RemoveAbility(FGameplayAbilitySpec& AbilitySpec);
	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void AddGameplayTag(FGameplayTag TagToAdd);
	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void AddGameplayTags(TArray<FGameplayTag> DefaultTags);
	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void RemoveGameplayTag(FGameplayTag TagToRemove);
	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		bool DoesCharacterHaveTag(FGameplayTag const& Tag);
	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void SetWeaponMesh(UQuestWeaponItem* Weapon);
	UFUNCTION(BlueprintImplementableEvent, Category = "QuestCharacterBase", meta = (DisplayName = "SetWeaponMesh"))
		void BP_SetWeaponMesh(UQuestWeaponItem* Weapon);

	/** This function must be overriden to avoid an abstract class error */
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override { AbilitySystemComponent->GetOwnedGameplayTags(TagContainer); return; }
	// old implmentation of GetOwnedGameplayTags { TagContainer = GameplayTags; return; }

	/** Function called by delegate when the character's health changes */
	UFUNCTION()
		void OnHealthChanged(float Health, float MaxHealth);

	/** Blueprint function that will be called by OnHealthChanged when character's health changes */
	UFUNCTION(BlueprintImplementableEvent, Category = "QuestCharacterBase", meta = (DisplayName = "OnHealthChanged"))
		void BP_OnHealthChanged(float Health, float MaxHealth);

	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void TurnTowardTarget(AQuestCharacterBase* TargetCharacter);

	UFUNCTION(BlueprintImplementableEvent, Category = "QuestCharacterBase", meta = (DisplayName = "TurnTowardTarget"))
	void BP_TurnTowardTarget(AQuestCharacterBase* Target);

	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void TurnTowardLocation(FVector Location);

	UFUNCTION(BlueprintImplementableEvent, Category = "QuestCharacterBase", meta = (DisplayName = "TurnTowardLocation"))
	void BP_TurnTowardLocation(FVector Location);

	/** Function called when character wants to make a melee attack */
	UFUNCTION()
		void MeleeAttack();

	/** Blueprint function called by MeleeAttack; the blueprint implementation should trigger the Melee gameplay ability */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "QuestCharacterBase", meta = (DisplayName = "MeleeAttack"))
		void BP_MeleeAttack();

	///** Functions used to retrieve the character's projectile spellcasting montage */
	//UFUNCTION(BlueprintCallable)
	//	UAnimMontage* GetProjectileSpellcastingMontage();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "QuestCharacterBase", meta = (DisplayName = "GetProjectileSpellcastingMontage"))
		UAnimMontage* BP_GetProjectileSpellcastingMontage();

	/** Function called when character wants to cast a spell */
	/** NOTE:  This function must be overridden in child classes because enemies handle spells different than party members */
	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		virtual void CastSpell(TSubclassOf<UQuestGameplayAbility> Ability);

	/** Function called when character finishes casting a spell */
	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		virtual void OnFinishedCastingSpell(TSubclassOf<UQuestGameplayAbility> Spell, bool bShouldBeRemovedFromMemorizedSpells);

	/** Blueprint event that gets called when the character dies */
	UFUNCTION(BlueprintImplementableEvent, Category = "QuestCharacterBase", meta = (DisplayName = "Die"))
		void BP_Die();

	/** Basic functions to implement the spellcasting system */
	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void SetSpellbookType();

	/** Moves character to start point on rotation actor such as whirlwind */
	UFUNCTION(BlueprintImplementableEvent, Category = QuestCharacterBase, meta = (DisplayName = MoveToStartPositionForRotationActor))
		void BP_MoveToStartPositionForRotationActor(FVector StartPosition);

	/** Called when the rotation component stops rotating; used to give the character a delay before rotating again */
	UFUNCTION(BlueprintImplementableEvent, Category = QuestCharacterBase, meta = (DisplayName = OnStopRotating))
		void BP_OnStopRotating();

	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void SetCharacterGroup(AQuestCharacterGroup* InCharacterGroup);

	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void CreateCharacterGroup();

	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void EnterCombat();

	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		virtual void LeaveCombat();

	UFUNCTION(BlueprintCallable, BlueprintPure,  Category = "QuestCharacterBase")
		bool IsAdverse(const AQuestCharacterBase* OtherActor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "QuestCharacterBase")
		bool IsAllied(const AQuestCharacterBase* OtherActor);

	/** Sets an auto order struct to used; called by order handling component once order is successfully completed */
	void SetAutoOrderAsUsed(TSoftClassPtr<UQuestOrder> Order);

	FOnDeath OnDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayTags")
		FGameplayTagContainer GameplayTags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		FGameplayTag FullHealthTag;
	// The tags that should initially be added to the character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		TArray<FGameplayTag> DefaultTags;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		UQuestAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		TArray<TSubclassOf<UQuestGameplayAbility>> AbilitiesToAcquire;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		UQuestAttributeSet* AttributeSetComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		UQuestDefaultAbilities* DefaultAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		UQuestRotationComponent* RotationComponent;
	//  Tells us whether the character's rotation component is currently rotating the character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spells)
		bool bIsRotating;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		ECharacterClass CharacterClass;
	/** Used to track whether this character has a spellbook */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		bool bIsSpellcaster;
	/** The Game Mode */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestCharacterBase")
		AQuestGameMode* GameMode;

	// Determines whether this character is hostile to the player
	// TODO:  Remove this as it should be irrelevant given the Affiliation ENUM
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		bool bIsHostile;

	// Describes the character's relationship with the player's party
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		ECharacterAffiliation Affiliation;

	// Sets the group of characters that fight with this character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		AQuestCharacterGroup* CharacterGroup;

	// Tells us whether this character is the leader of its CharacterGroup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		bool bIsLeader;

	// Tells us whether some event should be triggered when this character's group is defeated
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		bool bShouldDefeatTriggerEvent;

	// If this character is the leader of its CharacterGroup, how far to look for other group members
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		float GroupRange;

	// Tells us whether the character is dead
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
	bool bIsDead;

	// The weapon that the character is currently wielding
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		UQuestWeaponItem *EquippedWeapon;

	// The static mesh component of the EquippedWeapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		USkeletalMeshComponent* EquippedWeaponMesh;

	// The shield that the character is currently wielding
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		UQuestShieldItem* EquippedShield;

	// The static mesh component of the EquippedShield
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		USkeletalMeshComponent* EquippedShieldMesh;

	// The character's spellbook, if the character can cast spells
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spells)
		AQuestSpellbook* Spellbook;

	// Variable used to cache the spell the character is trying to cast 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spells)
		TSubclassOf<UQuestGameplayAbility> AttemptedSpell;

	// Variable to cache the character that this character's actions are directed toward
// TODO:  Delete this here and incorporate into AI system
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = QuestCharacterBase)
		AQuestCharacterBase* TargetActor;

	// Variable to cache the character's distance from intended location
	//  TODO:: Delete this here and incorporate into the AI system
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = QuestCharacterBase)
		float DistanceFromDestination;

	/** Variables for Order System */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = QuestCharacterBase)
	UQuestOrderHandlingComponent* OrderHandlingComponent;
	UQuestAutoOrderComponent* AutoOrderComponent;

	/** Orders for the character to follow automatically when the character enters combat */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = QuestCharacterBase)
	TArray<FQuestAutoOrder> AutoOrderArray;

	/** Default order, puts character at idle */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = QuestCharacterBase)
	TSoftClassPtr<UQuestDefaultOrder> DefaultOrder;

	/** Order that tells the character to use an ability */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = QuestCharacterBase)
		TSoftClassPtr<UQuestUseAbilityOrder> UseAbilityOrder;

	/** Variables for the AI perception component */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = QuestCharacterBase)
		float AISightRadius;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = QuestCharacterBase)
		float AILoseSightRadius;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = QuestCharacterBase)
		float AISightAge;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = QuestCharacterBase)
		float AIFieldOfView;


private:

	/** Sets up the character group, but is overriden for QuestCharacter class, which uses the Party instead */
	virtual void InitializeCharacterGroup();
	void AddMembersToCharacterGroup();





};
