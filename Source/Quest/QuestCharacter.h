// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "QuestCharacterBase.h"
#include "QuestCharacter.generated.h"

class AQuestStorage;
class AQuestMerchantCharacter;
class UQuestCharacterAttributeSet;
class UQuestSelectableComponent;

UCLASS(Blueprintable)
class AQuestCharacter : public AQuestCharacterBase
{
	GENERATED_BODY()

public:
	AQuestCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	/** Handles the functions applicable when a character in the party is selected */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UQuestSelectableComponent* SelectableComponent;


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacter")
		AQuestStorage* StorageChest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacter")
		AQuestMerchantCharacter* Merchant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacter")
		int32 Experience;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacter")
		int32 NextLevelExperience;


	void InteractWithTarget(AActor* InteractionTarget);

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

};

