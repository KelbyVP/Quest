// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "QuestRotationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUEST_API UQuestRotationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestRotationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestCharacterRotationActor)
		FVector InitialLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestCharacterRotationActor)
		float CurrentRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = QuestCharacterRotationActor)
		float Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestCharacterRotationActor)
		float StartingAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = QuestCharacterRotationActor)
		float MovementSpeedToStartingRotationPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = QuestCharacterRotationActor)
		float RotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = QuestCharacterRotationActor)
		float RotationHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = QuestCharacterRotationActor)
		bool bClockwise;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = QuestCharacterRotationActor)
		AActor* CharacterToRotate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = QuestCharacterRotationActor)
		AActor* ActorAtCenter;
	float ClockwiseFloat;
	FVector StartingRotationPosition;
	FVector CurrentPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bShouldRotate = false;
	bool bShouldMoveToStartingPosition = false;
	bool bHasStartingRotationPosition = false;

	// Sets the variables we will need to begin rotating the character
	UFUNCTION(BlueprintCallable)
		void StartRotating(float InRadius, float InMovementSpeedToStartingRotatingPosition, float InRotationSpeed,
			float InRotationHeight, bool bInClockwise, AActor* InActorAtCenter);

	// Rotates the character around the center position
	UFUNCTION(BlueprintCallable)
		void RotateActorAroundPoint();

	// Moves the character gradually to a point at which to begin rotating
	UFUNCTION(BlueprintCallable)
		void MoveActorToStartingRotationPosition(float DeltaTime);

	// Finds the tangent point at the center actor's radius so the transition from movement to rotation will be smooth
	void GetStartingRotationPosition();

	UFUNCTION(BlueprintCallable)
		void StopRotating();

	UFUNCTION()
		void OnCharacterDeath();
};
