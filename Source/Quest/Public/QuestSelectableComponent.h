// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestSelectableComponent.generated.h"

class AQuestCharacterBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelected);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUEST_API UQuestSelectableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestSelectableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FOnSelected OnSelected;
	
	UFUNCTION(BlueprintCallable, Category = QuestSelectableComponent)
		void ExecuteOnSelected();


};
