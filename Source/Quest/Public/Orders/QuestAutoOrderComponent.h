// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestOrderData.h"
#include "QuestAutoOrderComponent.generated.h"

class UQuestDefaultOrder;
class UQuestOrder;
class UQuestOrderHandlingComponent;

/**
 * This component automatically generates the next order for a character to execute
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUEST_API UQuestAutoOrderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestAutoOrderComponent();
	bool IssueAutoOrder(const TSoftClassPtr<UQuestOrder> Order);

	// Stores the character's default order 
	TSoftClassPtr<UQuestDefaultOrder> DefaultOrder;

	/** Tells us whether the character is in combat */
	bool bIsInCombat = false;

	/** Sets a ranged or melee attack order based on the character's equipped weapon */
	bool GetWeaponAttackOrder(TSoftClassPtr<UQuestOrder>& InOrder);

	UFUNCTION()
		void EnterCombat();
	UFUNCTION()
		void GenerateAutoOrder();



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:



	UFUNCTION()
		bool SelectAutoOrder(TSoftClassPtr<UQuestOrder>& InOrder);

		
};
