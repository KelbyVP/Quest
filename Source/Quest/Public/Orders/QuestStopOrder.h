// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Orders/QuestOrder.h"
#include "QuestStopOrder.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintType, Abstract, Blueprintable)
class QUEST_API UQuestStopOrder : public UQuestOrder
{
	GENERATED_BODY()

public:

	/** Default class constructor */
	UQuestStopOrder();

	virtual bool AreAutoOrdersAllowedDuringOrder() const;


	
};
