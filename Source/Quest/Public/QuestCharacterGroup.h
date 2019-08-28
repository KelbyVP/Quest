// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestCharacterGroup.generated.h"

class AQuestCharacterBase;

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

	/** Tells us whether this group is currently in combat */
	bool bIsInCombat;
};
