// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestCharacterBase.h"
#include "QuestCharacterGroup.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnterCombat);

UCLASS()
class QUEST_API AQuestCharacterGroup : public AActor
{
	GENERATED_BODY()
	

		/** 
		*	This class permits groups of characters to act together.
		*	The party class inherits from it.
		*/

public:	
	// Sets default values for this actor's properties
	AQuestCharacterGroup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	

	/** Array of characters that are part of this group */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestCharacterGroup")
		TArray<AQuestCharacterBase*> Members;

	/** The leader of this group */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestCharacterGroup")
		AQuestCharacterBase* Leader;

	/** The affiliation of this group with the player's party */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "QuestCharacterGroup")
		ECharacterAffiliation Affiliation = ECharacterAffiliation::IT_Neutral;


	/** Tells us whether this group is currently in combat */
	bool bIsInCombat;

	/** Array of character groups with whom this group is currently fighting */
	TArray<AQuestCharacterGroup*> AdverseGroupsInCombat;
	
	FOnEnterCombat OnEnterCombat;

	/** Adds a character to the group */
	UFUNCTION(BlueprintCallable, Category = "QuestCharacterGroup")
		void AddCharacter(AQuestCharacterBase* CharacterToAdd);

	/** Sets a leader for the group */
	UFUNCTION(BlueprintCallable, Category = "QuestCharacterGroup")
		void SetLeader(AQuestCharacterBase* NewLeader);

	UFUNCTION(BlueprintCallable, Category = "QuestCharacterGroup")
		void CheckShouldStartFighting(AQuestCharacterBase* CharacterToFight);


private:


};
