// Copyright 2019 Kelby Van Patten

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "QuestAbilitySystemComponent.h"
#include "GameplayAbility.h"
#include "QuestWeaponItem.h"
#include "QuestGameMode.h"
#include "QuestCharacterBase.generated.h"

class UQuestAttributeSet;
class USphereComponent;

UCLASS()
class QUEST_API AQuestCharacterBase : public ACharacter, public IAbilitySystemInterface
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		UQuestAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		UQuestAttributeSet* AttributeSetComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		FGameplayTag FullHealthTag;

	// Creates a sphere around the character that tells us when the player can interact with (usually melee attack) the enemy
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "QuestCharacterBase")
		USphereComponent* InteractionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		float InteractionSphereRadius;
	
	// Used to track distance and rotation to new location; may be useful for setting animation transitions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		float DistanceFromDestination;

	// Determines whether this character is hostile to the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		bool bIsHostile;

	// Identifies the character that this character is targeting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		AActor *TargetActor;

	//  Tells us whether this character is within range of the character that this character is trying to melee attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		bool bIsTargetWithinInteractionSphere;

	// Tells us whether the character is dead
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
	bool bIsDead;

	/** Tells us whether the game mode has shifted to combat mode, which allows characters to attack automatically */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacterBase")
		bool bIsCombatModeActive;

	// The weapon that the character is currently wielding
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		UQuestWeaponItem *EquippedWeapon;

	// The static mesh component of the EquippedWeapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		USkeletalMeshComponent* EquippedWeaponMesh;

	/** The Game Mode */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestCharacterBase")
		AQuestGameMode* GameMode;

	/** Basic functions to implement the ability system */
	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void AcquireAbility(TSubclassOf<UGameplayAbility>AbilityToAcquire);
	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void AddGameplayTag(FGameplayTag TagToAdd);
	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void RemoveGameplayTag(FGameplayTag TagToRemove);

	/** Function called by delegate when the character's health changes */
	UFUNCTION()
		void OnHealthChanged(float Health, float MaxHealth);

	/** Blueprint function that will be called by OnHealthChanged when character's health changes */
	UFUNCTION(BlueprintImplementableEvent, Category = "QuestCharacterBase", meta = (DisplayName = "OnHealthChanged"))
		void BP_OnHealthChanged(float Health, float MaxHealth);

	/** Functions called when another actor moves into or out of the Interaction Sphere */
	UFUNCTION()
		virtual void OnInteractionSphereBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		virtual void OnInteractonSphereEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Function called by delegate when the Combat Mode changes */
	UFUNCTION()
		void OnCombatModeChanged(bool NewbIsCombatModeActive);

	/** Function called when character wants to make a melee attack */
	UFUNCTION()
		void MeleeAttack();

	/** Blueprint function called by MeleeAttack; the blueprint implementation should trigger the Melee gameplay ability */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "QuestCharacterBase", meta = (DisplayName = "MeleeAttack"))
		void BP_MeleeAttack();

	/** Blueprint event that gets called when the character dies */
	UFUNCTION(BlueprintImplementableEvent, Category = "QuestCharacterBase", meta = (DisplayName = "Die"))
		void BP_Die();

	/** Function that clears the TargetActor value */
	UFUNCTION(BlueprintCallable, Category = "QuestCharacterBase")
		void SetTargetActorToNull();
};
