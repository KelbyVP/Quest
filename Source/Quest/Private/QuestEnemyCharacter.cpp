// Copyright 2019 Kelby Van Patten


#include "QuestEnemyCharacter.h"
#include "QuestAttributeSet.h"


AQuestEnemyCharacter::AQuestEnemyCharacter()
{
	bIsHostile = true;
	Affiliation = ECharacterAffiliation::IT_Hostile;
	bIsLeader = true;
}

void AQuestEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();	

}
