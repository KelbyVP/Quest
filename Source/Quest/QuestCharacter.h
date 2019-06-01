// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "QuestCharacterBase.h"
#include "QuestCharacter.generated.h"

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

	void SelectTargetCharacterToAttack();
	TArray<FHitResult> ScanForNearbyPawns();

	/** Identifies which pawns are hostile and alive */
	TArray<AQuestCharacterBase*> GetEnemiesFromPawnHits(TArray<FHitResult> OutHits);

	/** Identifies which enemy is closest to this character */
	AQuestCharacterBase* SelectClosestEnemy(TArray<AQuestCharacterBase*> LocalLiveEnemies);

	void AutoAttack();

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "QuestCharacter")
		bool bIsReadyForNextAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestCharacter")
		float AttackCooldownTimer;



	void InteractWithTarget(AActor* InteractionTarget);

	UFUNCTION(BlueprintCallable, Category = "QuestCharacter")
		void MoveToTarget(AActor* MoveTarget);

	UFUNCTION(BlueprintCallable, Category = "QuestCharacter")
	void SetbIsReadyForNextAttack(bool NewbIsReadyForNextAttack);

	UFUNCTION(BlueprintCallable, Category = "QuestCharacter")
		bool GetbIsReadyForNextAttack();

	UFUNCTION()
	void OnMeleeEnd();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "QuestCharacter", meta = (DisplayName = "OnMeleeEnd"))
		void BP_OnMeleeEnd();

	virtual void OnInteractionSphereBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnInteractonSphereEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};

