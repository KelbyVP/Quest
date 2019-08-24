// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatGroup.generated.h"

class AQuestCharacterBase;
UCLASS()
class QUEST_API ACombatGroup : public AActor
{
	GENERATED_BODY()
	
	/** 
	*	This class is a group of characters that fight together.  The BP_Party class inherits from this, 
	*	as well as enemy and friendly NPC groups 
	*/

public:	
	// Sets default values for this actor's properties
	ACombatGroup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Array of characters in the combat group
	TArray<AQuestCharacterBase*> Members;

	// Tells us whether this combat group is in combat
	bool bIsInCombat;

	UFUNCTION()
	void EnterCombat();

	UFUNCTION()
	void EndCombat();
};
