// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "QuestSpectatorPawn.generated.h"

class AQuestCharacterBase;

/**
 * 
 */
UCLASS()
class QUEST_API AQuestSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()

public:

		AQuestSpectatorPawn();

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float DefaultCameraBoomLength;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		FRotator DefaultCameraRotation;

	/**
	 *  Logic that smoothly moves the Spectator Pawn to center on a selected character
	 */
	// Called by the Character Slot Widget when double-clicked to initiate the centering process
	UFUNCTION(BlueprintCallable, Category = QuestSelectableComponent)
		void CenterOnCharacter(AQuestCharacterBase* SelectedCharacter);
	// Called by the Tick to incrementally move the Spectator Pawn toward the selected character location
	void MoveToSelectedCharacterLocation(float DeltaTime);
	// Used by the Tick to determine whether to move the Spectator Pawn
	FVector DestinationLocation;
	bool bShouldMovePawnTowardSelectedCharacter;
};
