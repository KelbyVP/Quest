// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestSelectableComponent.h"
#include "QuestCharacterBase.h"

// Sets default values for this component's properties
UQuestSelectableComponent::UQuestSelectableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UQuestSelectableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UQuestSelectableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuestSelectableComponent::ExecuteOnSelected()
{
	// TODO:  This is interesting, but so far I don't think anyone is subscribing to this broadcast.  Might want to delete?  If so, we could delete the class?
	OnSelected.Broadcast();
}

