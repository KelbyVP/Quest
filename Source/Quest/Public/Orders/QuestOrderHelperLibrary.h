// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QuestOrderErrorTags.h"
#include "QuestOrderHelperLibrary.generated.h"

class UQuestOrder;
class AQuestCharacterBase;

/**
 * 
 */
UCLASS()
class QUEST_API UQuestOrderHelperLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Checks whether the ordered actor can obey the order */
	UFUNCTION(BlueprintPure, Category = "QuestOrderHelperLibrary")
		static bool CanObeyOrder(TSoftClassPtr<UQuestOrder> OrderType, const AActor* OrderedActor);

	/**  Tells us whether to restart the tree when new order of same type issued */
	UFUNCTION(BlueprintPure, Category = "QuestOrderHelperLibrary")
		static bool ShouldRestartBehaviorTree(TSoftClassPtr<UQuestOrder> OrderType);

	/** Gets an order's target type */
	static EQuestOrderTargetType GetTargetType(TSoftClassPtr<UQuestOrder> OrderType);

	/** Gets an order's target scoring method */
	static EQuestOrderTargetScoringMethod GetTargetScoringMethod(TSoftClassPtr<UQuestOrder> OrderType);

	/** Gets the range in which to look for possible targets */
	static float GetTargetAcquisitionRange(TSoftClassPtr<UQuestOrder> OrderType);

	/** Gets an order's max range */
	static float GetRange(TSoftClassPtr<UQuestOrder> OrderType);

	/** Gets an order's cancellation policy */
	static EQuestOrderCancellationPolicy GetCancellationPolicy(TSoftClassPtr<UQuestOrder> OrderType);

	/** Gets behavior tree associated with the order */
	static UBehaviorTree* GetBehaviorTree(TSoftClassPtr<UQuestOrder> OrderType);

	/** Chooses the best target suitable for the order */
	static AQuestCharacterBase* SelectTarget(const AQuestCharacterBase* OrderedCharacter, TSoftClassPtr<UQuestOrder> OrderType);

	static TArray<AQuestCharacterBase*> GetHostileTargetsInRange(const AQuestCharacterBase* OrderedCharacter, float Radius);

	/** Chooses the closest target to the ordered character */
	static AQuestCharacterBase* SelectClosestTarget(const AActor* OrderedCharacter, TArray<AQuestCharacterBase*> TargetCharacters);

	static AQuestCharacterBase* SelectClosestHostileTarget(const AQuestCharacterBase* OrderedCharacter, TSoftClassPtr<UQuestOrder> OrderType);

	static AQuestCharacterBase* SelectClosestHostileLeaderInAcquisitionRange(const AQuestCharacterBase* OrderedCharacter, TSoftClassPtr<UQuestOrder> OrderType);

	/** 
	*	These functions assumes that higher level characters are the most powerful, and that within a given level,
	*	the most powerful character is the one with the most health
	*/
	static AQuestCharacterBase* SelectMostPowerfulAdversaryInAcquisitionRange(const AQuestCharacterBase* OrderedCharacter, TSoftClassPtr<UQuestOrder> OrderType);

	/** This function returns the most powerful adverse spellcaster, but if there are no spellcasters, returns most powerful adversary */
	static AQuestCharacterBase* SelectMostPowerfulAdverseSpellcasterInAcquisitionRange(const AQuestCharacterBase* OrderedCharacter, TSoftClassPtr<UQuestOrder> OrderType);

	/** This function returns the most powerful allied leader in target acquisition range, and if there are no allied leaders, returns most powerful allied character */
	static AQuestCharacterBase* SelectMostPowerfulAlliedLeaderInAcquisitionRange(const AQuestCharacterBase* OrderedCharacter, TSoftClassPtr<UQuestOrder> OrderType);

	/** 
	*	This function identifies the most powerful character in an array.  It assumes higher level characters are more powerful.
	*	Within a given level, it assumes characters with more current health are more powerful 
	*/
	static AQuestCharacterBase* GetMostPowerfulCharacterInArray(TArray<AQuestCharacterBase*>& CharacterArray);

	static TArray<AQuestCharacterBase*> GetCharacterssInRange(const AQuestCharacterBase* OrderedCharacter, float Radius);






};
