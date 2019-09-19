// Copyright 2019 Kelby Van Patten, All Rights Reserved


#include "QuestCharacterGroup.h"
#include "QuestAIController.h"
#include "QuestCharacterBase.h"
#include "QuestOrderHelperLibrary.h"

// Sets default values
AQuestCharacterGroup::AQuestCharacterGroup()
{
	bIsInCombat = false;
	bShouldDefeatTriggerEvent = false;
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

	// If this character has been a leader before, but this group already has a leader, this character is no longer a leader
	if (IsValid(Leader))
	{
		CharacterToAdd->bIsLeader = false;
	}
}

void AQuestCharacterGroup::SetLeader(AQuestCharacterBase* NewLeader)
{
	if (IsValid(NewLeader))
	{
		Leader = NewLeader;
		Leader->bIsLeader = true;
		Affiliation = Leader->Affiliation;
	}
}

void AQuestCharacterGroup::CheckShouldStartFighting(AQuestCharacterBase* CharacterToFight)
{
	/** Don't start fighting if other character is neutral, on our side, or dead */
	if (IsValid(CharacterToFight) && IsValid(Leader)) // NOTE:  This will give a read access violation if there isn't a leader for a group and therefore no group
	{
		if (!CharacterToFight->IsAdverse(Leader) ||
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

		///** Tell the other group to check whether it should start fighting */
		// Note:  I have disabled this for now so that you can sneak attack someone if they don't perceive you;
		if (IsValid(Leader) && IsValid(CharacterToFight) && IsValid(CharacterToFight->CharacterGroup))
		{
			AQuestAIController* AIController = Cast<AQuestAIController>(CharacterToFight->GetController());
			if (AIController) 
			{
				CharacterToFight->CharacterGroup->CheckShouldStartFighting(Leader);
			}
		}
	}
}

void AQuestCharacterGroup::OnMemberDeath(AQuestCharacterBase* DeadCharacter)
{
	if (DeadCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestCharacterGroup::OnMemberDeath: DeadCharacter variable is nullptr"));
		return;
	}

	/** Check whether there are any living members */
	bool bDoesGroupHaveAnyLivingMembers = false;
	if (Members.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestCharacterGroup::OnMemberDeath: No members in %s' group! "), *DeadCharacter->GetName());
		return;
	}
	for (auto& Member : Members)
	{
		if (Member && !Member->bIsDead)
		{
			bDoesGroupHaveAnyLivingMembers = true;
			break;
		}
	}

	/** If there are not any living members, tell groups in combat that this group is defeated */
	if (!bDoesGroupHaveAnyLivingMembers)
	{
		bIsInCombat = false;
		for (auto& Group : AdverseGroupsInCombat)
		{
			if (IsValid(Group))
			{
				Group->OnCharacterGroupDefeated(this);
			}
		}
		if (bShouldDefeatTriggerEvent)
		{
			// TODO:  Implement functionality to trigger event if this bool is true
		}
		return;
	}

	/** If there are still living members, but the leader died, pick a new leader from those still living */

	if (DeadCharacter == Leader)
	{
		TArray<AQuestCharacterBase*> LivingMembers;
		for (auto& Member : Members)
		{
			if (Member && !Member->bIsDead)
			{
				LivingMembers.AddUnique(Member);
			}
		}
		if (LivingMembers.Num() > 0)
		{
			SetLeader(UQuestOrderHelperLibrary::GetMostPowerfulCharacterInArray(LivingMembers));
		}
	}
}

void AQuestCharacterGroup::OnCharacterGroupDefeated(AQuestCharacterGroup* DefeatedGroup)
{
	if (!IsValid(DefeatedGroup))
	{
		return;
	}

	/** Remove defeated group from array of groups with whom we are in combat */
	if (AdverseGroupsInCombat.Contains(DefeatedGroup))
	{
		AdverseGroupsInCombat.Remove(DefeatedGroup);
	}

	/** If there are no more groups in combat, end combat */

	if (AdverseGroupsInCombat.Num() <= 0)
	{
		EndCombat();
	}
}

void AQuestCharacterGroup::EndCombat()
{
	bIsInCombat = false;
	OnLeaveCombat.Broadcast();
}

