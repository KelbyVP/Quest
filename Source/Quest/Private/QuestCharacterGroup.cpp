// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestCharacterGroup.h"
#include "QuestCharacterBase.h"

// Sets default values
AQuestCharacterGroup::AQuestCharacterGroup()
{
	bIsInCombat = false;
}

// Called when the game starts or when spawned
void AQuestCharacterGroup::BeginPlay()
{
	Super::BeginPlay();	
}

void AQuestCharacterGroup::AddCharacter(AQuestCharacterBase* CharacterToAdd)
{
	Members.AddUnique(CharacterToAdd);
	CharacterToAdd->SetCharacterGroup(this);
}

void AQuestCharacterGroup::SetLeader(AQuestCharacterBase* NewLeader)
{
	Leader = NewLeader;
	Affiliation = Leader->Affiliation;
}

void AQuestCharacterGroup::CheckShouldStartFighting(AQuestCharacterBase* CharacterToFight)
{
	/** Don't start fighting if other character is neutral, on our side, or dead */
	if (CharacterToFight != nullptr && Leader != nullptr)
	{
		if (CharacterToFight->Affiliation == ECharacterAffiliation::IT_Neutral || 
			CharacterToFight->Affiliation == Leader->Affiliation ||
			CharacterToFight->bIsDead
			)
		{
			return;
		}

		/** Don't need to start fighting if we are already fighting this group */
		if (AdverseGroupsInCombat.Contains(CharacterToFight->CharacterGroup))
		{
			return;
		}

		/** Start fighting this group */
		AdverseGroupsInCombat.Add(CharacterToFight->CharacterGroup);

		/** if we aren't already in combat, start */
		if (!bIsInCombat)
		{
			bIsInCombat = true;
			OnEnterCombat.Broadcast();
		}

		/** Tell the other group to check whether it should start fighting */
		if (Leader)
		{
			CharacterToFight->CharacterGroup->CheckShouldStartFighting(Leader);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("QuestCharacterGroup::CheckShouldStartFighting: %s Got stuck on null pointers!"), *GetName());
	}
}

