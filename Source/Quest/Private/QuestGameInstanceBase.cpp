// Copyright 2019 Kelby Van Patten


#include "QuestGameInstanceBase.h"
//#include "Math/UnrealMathUtility.h"

void UQuestGameInstanceBase::IncreaseGold(int Amount)
{
	Gold += Amount;
	FMath::Clamp(Gold, 0, MaxGold);
	UpdateGold();
}

void UQuestGameInstanceBase::DecreaseGold(int Amount)
{
	Gold -= Amount;
	FMath::Clamp(Gold, 0, MaxGold);
	UpdateGold();
}
