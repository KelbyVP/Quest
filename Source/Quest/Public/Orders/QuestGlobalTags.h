// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QuestGlobalTags.generated.h"

/**
 *	Global tags that need to be accessible in C++
 */
UCLASS()
class QUEST_API UQuestGlobalTags : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	// --------------------------------------------------------------------------------------------
	// Ability activation failure tags
	// --------------------------------------------------------------------------------------------
	
	/** The ability cannot be activated because there is no target */
	static const FGameplayTag& AbilityActivationFailure_NoTarget();

	// --------------------------------------------------------------------------------------------
	// Status tags
	// --------------------------------------------------------------------------------------------

	/** Gets the status.alive tag that tells us whether the character is alive */
	static const FGameplayTag& Status_Alive();

	/** Whether the character can detect stealth units */
	static const FGameplayTag& Status_Detector();

	/** Whether the character is hidden from its enemies */
	static const FGameplayTag& Status_Stealthed();


	// --------------------------------------------------------------------------------------------
	// Relationship tags
	// --------------------------------------------------------------------------------------------

	static const FGameplayTag& Relationship_Visible();

};
