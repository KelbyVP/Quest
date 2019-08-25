// Copyright 2019 Kelby Van Patten

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "QuestAbilitySystemComponent.h"
#include "GameplayAbility.h"
#include "QuestWeaponItem.h"
#include "QuestShieldItem.h"
#include "QuestGameMode.h"
#include "QuestCharacterBase.generated.h"

class UQuestAttributeSet;
class USphereComponent;
class UQuestGameplayAbility;
class AQuestSpellbook;
class AQuestCharacterRotationActor;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayTags")
		FGameplayTagContainer GameplayTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		UQuestAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		TArray<TSubclassOf<UQuestGameplayAbility>> AbilitiesToAcquire;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		UQuestAttributeSet* AttributeSetComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		FGameplayTag FullHealthTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		ECharacterClass CharacterClass;
	/** Used to track whether this character has a spellbook */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		bool bIsSpellcaster;
	/** The Game Mode */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestCharacterBase")
		AQuestGameMode* GameMode;

	// Determines whether this character is hostile to the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		bool bIsHostile;

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

	/** Basic functions to implement the ability system */
	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void AcquireAbility(TSubclassOf<UGameplayAbility>AbilityToAcquire);
	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void AddGameplayTag(FGameplayTag TagToAdd);
	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void RemoveGameplayTag(FGameplayTag TagToRemove);
	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
	bool DoesCharacterHaveTag(FGameplayTag const& Tag);

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override { TagContainer = GameplayTags; return; }


	/** Function called by delegate when the character's health changes */
	UFUNCTION()
		void OnHealthChanged(float Health, float MaxHealth);

	/** Blueprint function that will be called by OnHealthChanged when character's health changes */
	UFUNCTION(BlueprintImplementableEvent, Category = "QuestCharacterBase", meta = (DisplayName = "OnHealthChanged"))
		void BP_OnHealthChanged(float Health, float MaxHealth);

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

	UFUNCTION(BlueprintImplementableEvent, Category = QuestCharacterBase, meta = (DisplayName = MoveToStartPositionForRotationActor))
		void BP_MoveToStartPositionForRotationActor(FVector StartPosition, AQuestCharacterRotationActor* RotationActor);

	// Variable to cache the character that this character's actions are directed toward
	// TODO:  Delete this here and incorporate into AI system
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = QuestCharacterBase)
		AQuestCharacterBase* TargetActor;

	// Variable to cache the character's distance from intended location
	//  TODO:: Delete this here and incorporate into the AI system
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = QuestCharacterBase)
	float DistanceFromDestination;



};
