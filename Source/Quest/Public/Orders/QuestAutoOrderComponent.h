// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestAutoOrderComponent.generated.h"

/**
 * Manages orders that should be automatically issued for a character
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUEST_API UQuestAutoOrderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestAutoOrderComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
};
