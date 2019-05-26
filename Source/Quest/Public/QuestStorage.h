// Copyright 2019 Kelby Van Patten

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestStorage.generated.h"

class AQuestPlayerController;

UCLASS()
class QUEST_API AQuestStorage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestStorage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** A blueprint function that allows the player to move items between a storage and the player inventory */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "OnInteract"), Category = "QuestPlayerController")
		void OnInteract(AQuestPlayerController* PlayerController);

};
