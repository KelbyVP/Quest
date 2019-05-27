// Copyright 2019 Kelby Van Patten


#include "QuestStorage.h"
#include "Components/BoxComponent.h"

// Sets default values
AQuestStorage::AQuestStorage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Sets up the InteractionBox
	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	InteractionBoxExtents = FVector(130, 80, 80);
	InteractionBox->SetupAttachment(RootComponent);
	InteractionBox->SetBoxExtent(InteractionBoxExtents);
}

// Called when the game starts or when spawned
void AQuestStorage::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQuestStorage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

