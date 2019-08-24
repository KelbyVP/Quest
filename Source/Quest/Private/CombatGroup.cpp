// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "CombatGroup.h"
#include "QuestCharacterBase.h"

// Sets default values
ACombatGroup::ACombatGroup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACombatGroup::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACombatGroup::EnterCombat()
{
	if (Members.Num() > 0)
	{

	}
}



