// Copyright 2019 Kelby Van Patten

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "QuestGATargetActorGroundSelect.generated.h"

class UDecalComponent;
class UScenecomponent;
class AQuestPlayerController;

/**
 * 
 */

UCLASS()
class QUEST_API AQuestGATargetActorGroundSelect : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:

	// Class constructor
	AQuestGATargetActorGroundSelect();

	// Called when instanced
	virtual void BeginPlay() override;

	// Called when destroyed 
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;


	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	// Override parent function to begin targeting by setting the relevant ability
	virtual void StartTargeting(UGameplayAbility* Ability) override;

	// Get the location under the cursor
	UFUNCTION(BlueprintCallable, Category = "GroundSelect")
		bool GetCursorLocation(FVector& CursorLocation);
	
	// Override parent function to see what actors are within the radius of the cursor location
	virtual void ConfirmTargetingAndContinue() override;

	// The radius of the targeted area in which to look for target actors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "GroundSelect")
		float Radius;

	// The decal that appears on the ground when targeting
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GroundSelect")
		UDecalComponent* Decal;

	// The root component of the decal
	USceneComponent* RootComp;

	// The player controller
	AQuestPlayerController* PlayerController;
};
