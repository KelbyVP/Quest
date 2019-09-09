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
class UQuestAttributeSet;
class UQuestAutoOrderComponent;
class UQuestDefaultOrder;
class UQuestGameplayAbility;
class UQuestOrder;
class UQuestOrderHandlingComponent;
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

	/** Blueprint event that gets called when the character dies */
	UFUNCTION(BlueprintImplementableEvent, Category = "QuestCharacterBase", meta = (DisplayName = "Die"))
		void BP_Die();

	/** Basic functions to implement the spellcasting system */
	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void SetSpellbookType();

	/** Moves character to start point on rotation actor such as whirlwind */
	UFUNCTION(BlueprintImplementableEvent, Category = QuestCharacterBase, meta = (DisplayName = MoveToStartPositionForRotationActor))
		void BP_MoveToStartPositionForRotationActor(FVector StartPosition, AQuestCharacterRotationActor* RotationActor);

	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void SetCharacterGroup(AQuestCharacterGroup* InCharacterGroup);

	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void CreateCharacterGroup();

	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void EnterCombat();

	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void LeaveCombat();

	UFUNCTION(BlueprintCallable, BlueprintPure,  Category = "QuestCharacterBase")
		bool IsAdverse(const AQuestCharacterBase* OtherActor);

	/** Sets an auto order struct to used; called by order handling component once order is successfully completed */
	void SetAutoOrderAsUsed(TSoftClassPtr<UQuestOrder> Order);


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

	//  Tells us whether this character is currently spinning in a rotation actor, so that it does not get picked up by another
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spells)
		bool bIsCapturedByRotationActor;

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

	// Variable to cache the character that this character's actions are directed toward
// TODO:  Delete this here and incorporate into AI system
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = QuestCharacterBase)
		AQuestCharacterBase* TargetActor;

	// Variable to cache the character's distance from intended location
	//  TODO:: Delete this here and incorporate into the AI system
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = QuestCharacterBase)
		float DistanceFromDestination;

	/** Variables for Order System */
	UQuestOrderHandlingComponent* OrderHandlingComponent;
	UQuestAutoOrderComponent* AutoOrderComponent;

	/** Orders for the character to follow automatically when the character enters combat */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = QuestCharacterBase)
	TArray<FQuestAutoOrder> AutoOrderArray;

	/** Default order, puts character at idle */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = QuestCharacterBase)
	TSoftClassPtr<UQuestDefaultOrder> DefaultOrder;

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
