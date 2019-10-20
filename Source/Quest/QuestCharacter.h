// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "QuestCharacterBase.h"
#include "RegionReputation.h"
#include "QuestCharacter.generated.h"

class AQuestStorage;
class AQuestMerchantCharacter;
class UQuestCharacterAttributeSet;
class UQuestHoldOrder;
class UQuestSelectableComponent;

UCLASS(Blueprintable)
class AQuestCharacter : public AQuestCharacterBase
{
	GENERATED_BODY()

public:

	AQuestCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	// Override parent function to ensure it does not interrupt a player directed order
	virtual void LeaveCombat() override;

	// Override parent function to implement player-directed spellcasting
	virtual void CastSpell(TSubclassOf<UQuestGameplayAbility> Ability) override;

private:


	/** Handles the functions applicable when a character in the party is selected */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UQuestSelectableComponent* SelectableComponent;

	//  I have overridden the overriding because I want the QuestCharacter to function outside the party before they become a member
	/** Override the parent function to do nothing because the QuestCharacter class uses a party instead */
	//  void InitializeCharacterGroup() override;


public:

	/** Variable for temporarily storing storage chest reference when storage chest is open */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacter")
		AQuestStorage* StorageChest;

	/** Variable for temporarily storing merchant reference when shop is open */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacter")
		AQuestMerchantCharacter* Merchant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacter")
		int32 Experience;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacter")
		int32 NextLevelExperience;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacter")
		TSoftClassPtr<UQuestHoldOrder> HoldOrder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacter")
		TArray<FRegionReputation> Reputation;

	void OnLeaveStorage();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "QuestCharacter", meta = (DisplayName = "OnLeaveStorage"))
		void BP_OnLeaveStorage();

	void OnLeaveMerchant();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "QuestCharacter", meta = (DisplayName = "OnLeaveMerchant"))
		void BP_OnLeaveMerchant();

	// If we have enough experience to level up, increment level and then call BP_OnLevelUp
	UFUNCTION(BlueprintCallable, Category = "QuestCharacter")
		void TryLevelUp();

	// Trigger the Blueprint Event that gets called when we level up
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "QuestCharacter", meta = (DisplayName = "OnLevelUp"))
		void BP_OnLevelUp();

	//  Add reputation points for the relevant region
	UFUNCTION(BlueprintCallable, Category = "QuestCharacter")
		void AddReputationPoints(FRegionReputation ReputationToAdd);

};

