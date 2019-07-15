// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestSpellbook.generated.h"

UCLASS()
class QUEST_API AQuestSpellbook : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQuestSpellbook();

protected:

public:

	// Sets the type of spells based on character class of owning character
	//ECharacterClass SpellType;

};