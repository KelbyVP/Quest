// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestSpells.h"
#include "QuestGameplayAbility.h"
#include "QuestSpellbook.generated.h"

USTRUCT(BlueprintType)
struct FMemorizedSpellStruct
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LearnedSpellStruct")
	TSubclassOf<class UQuestGameplayAbility> Spell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LearnedSpellStruct)
	bool bCanBeCast;

	FMemorizedSpellStruct();
};

UCLASS()
class QUEST_API AQuestSpellbook : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestSpellbook();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	ESpellType SpellbookType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestSpellbook)
	TArray<TSubclassOf<class UQuestGameplayAbility>> LearnedSpells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestSpellbook)
	TArray<FMemorizedSpellStruct> MemorizedSpells;

};
