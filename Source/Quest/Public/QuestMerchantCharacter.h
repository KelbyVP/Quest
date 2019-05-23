// Copyright 2019 Kelby Van Patten

#pragma once

#include "CoreMinimal.h"
#include "QuestCharacterBase.h"
#include "QuestMerchantCharacter.generated.h"

class AQuestPlayerController;
/**
 * 
 */
UCLASS()
class QUEST_API AQuestMerchantCharacter : public AQuestCharacterBase
{
	GENERATED_BODY()
	
public:
	AQuestMerchantCharacter();

	/** A blueprint function that allows the player to buy and sell goods with a merchant */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "OnInteract"), Category = "QuestPlayerController")
		void OnInteract(AQuestPlayerController *PlayerController);
};
