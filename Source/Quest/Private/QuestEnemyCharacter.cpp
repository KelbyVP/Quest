// Copyright 2019 Kelby Van Patten


#include "QuestEnemyCharacter.h"
#include "QuestAttributeSet.h"
#include "QuestGlobalTags.h"


AQuestEnemyCharacter::AQuestEnemyCharacter()
{
	bIsHostile = true;
	Affiliation = ECharacterAffiliation::IT_Hostile;
	bIsLeader = true;
	DefaultTags.AddUnique(UQuestGlobalTags::Relationship_Hostile());
}

void AQuestEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();	

}
