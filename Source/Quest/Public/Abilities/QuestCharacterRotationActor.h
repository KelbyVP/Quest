// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestCharacterRotationActor.generated.h"

UCLASS(BlueprintType, Blueprintable)
class QUEST_API AQuestCharacterRotationActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestCharacterRotationActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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

	UFUNCTION(BlueprintImplementableEvent)
		void BP_StopRotating();

	UFUNCTION()
	void OnCharacterDeath();

	UFUNCTION(BlueprintImplementableEvent)
		void BP_OnCharacterDeath();

};
