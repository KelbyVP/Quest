#include "QuestOrderErrorTags.h"

bool FQuestOrderErrorTags::IsEmpty() const
{
	return MissingTags.IsEmpty() && BlockingTags.IsEmpty() && ErrorTags.IsEmpty();
}

FString FQuestOrderErrorTags::ToString() const
{
	FString ErrorTagsString;
	ErrorTagsString += TEXT("Missing Tags: ");
	ErrorTagsString += MissingTags.ToString();

	ErrorTagsString += TEXT("Blocking Tags: ");
	ErrorTagsString += BlockingTags.ToString();

	ErrorTagsString += TEXT("Error Tags: ");
	ErrorTagsString += ErrorTags.ToString();

	return ErrorTagsString;
}
