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
	CharacterToAdd->CharacterGroup = this;
}

void AQuestCharacterGroup::SetLeader(AQuestCharacterBase* NewLeader)
{
	Leader = NewLeader;
	Affiliation = Leader->Affiliation;
}

void AQuestCharacterGroup::CheckShouldStartFighting(AQuestCharacterBase* CharacterToFight)
{
	/** Don't start fighting if other character is neutral or on our side */
	if (CharacterToFight != nullptr && Leader != nullptr)
	{
		if (CharacterToFight->Affiliation == ECharacterAffiliation::IT_Neutral 
			|| CharacterToFight->Affiliation == Leader->Affiliation)
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
		FString LeaderName;
		if (Leader)
		{
			LeaderName = Leader->GetName();
		}
		else
		{
			LeaderName = FString(TEXT("NoName"));
		}

		FString OtherName = CharacterToFight->GetName();
		FString ThisGroup = GetName();
		UE_LOG(LogTemp, Warning, TEXT("QuestCharacterGFroup::ShouldStartFighting %s added %s to an AdverseGroupsInCombat array called %s!"), *LeaderName, *OtherName, *ThisGroup);
	}

}

