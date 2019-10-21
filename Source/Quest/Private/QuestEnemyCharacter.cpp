// Copyright 2019 Kelby Van Patten


#include "QuestEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "QuestAttributeSet.h"
#include "QuestPlayerController.h"
#include "QuestQuestManager.h"
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

void AQuestEnemyCharacter::Die()
{
	AQuestPlayerController* Controller = Cast<AQuestPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (Controller)
	{
		Controller->QuestManager->OnEnemyKilled(this);
	}
	Super::Die();
}
