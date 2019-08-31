// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestCharacterGroup.h"
#include "QuestCharacterBase.h"

// Sets default values
AQuestCharacterGroup::AQuestCharacterGroup()
{
}

// Called when the game starts or when spawned
void AQuestCharacterGroup::BeginPlay()
{
	Super::BeginPlay();	
}

void AQuestCharacterGroup::AddCharacter(AQuestCharacterBase* CharacterToAdd)
{
	Members.AddUnique(CharacterToAdd);
	CharacterToAdd->CharacterGroup = this;
}

void AQuestCharacterGroup::SetLeader(AQuestCharacterBase* NewLeader)
{
	Leader = NewLeader;
	FString LeaderText = Leader->GetName();
	UE_LOG(LogTemp, Warning, TEXT("QuestCharacterGroup::SetLeader: leader has been set as %s"), *LeaderText);
}

