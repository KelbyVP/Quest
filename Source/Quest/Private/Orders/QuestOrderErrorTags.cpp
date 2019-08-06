#include "QuestOrderErrorTags.h"



bool FQuestOrderErrorTags::IsEmpty() const
{
	return (MissingTags.IsEmpty() && BlockingTags.IsEmpty() && ErrorTags.IsEmpty());
}

FString FQuestOrderErrorTags::ToString() const
{
	FString OrderErrorText;
	OrderErrorText += TEXT("MissingTags: ");
	OrderErrorText += MissingTags.ToString();

	OrderErrorText += TEXT(", Blocking Tags: ");
	OrderErrorText += BlockingTags.ToString();

	OrderErrorText += TEXT(", Error Tags: ");
	OrderErrorText += ErrorTags.ToString();

	return OrderErrorText;
}
