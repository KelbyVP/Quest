// Copyright 2019 Kelby Van Patten

#pragma once

#include "CoreMinimal.h"
#include "QuestCharacterBase.h"
#include "QuestMerchantCharacter.generated.h"

class AQuestPlayerController;
class UQuestOrder;
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

	// How close the character must be to interact with the storage
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "QuestMerchantCharacter")
		float InteractionRange;

	/** The order used to open the merchant's shop */
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "QuestMerchantCharacter")
		TSoftClassPtr<UQuestOrder> OpenShopOrder;
};
