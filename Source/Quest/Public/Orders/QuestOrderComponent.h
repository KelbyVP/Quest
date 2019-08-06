// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestOrderData.h"
#include "Math/Vector.h"
#include "QuestOrderComponent.generated.h"

class UQuestSelectableComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FQuestOrderComponentOrderQueueClearedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestOrderComponentOrderChangedSignature, const FQuestOrderData&, NewOrder);

/**
 * Manages orders for a character
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUEST_API UQuestOrderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestOrderComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	/** Event when character gets a new order */
	virtual void NotifyOnOrderChanged(const FQuestOrderData& NewOrder);
	UPROPERTY(BlueprintAssignable, Category = "QuestOrderComponent")
		FQuestOrderComponentOrderChangedSignature OnOrderChanged;

	/** Instructs this character to execute an order */
	void IssueOrder(const FQuestOrderData& Order);

	/** Event broadcast when order queue is cleared */
	UPROPERTY(BlueprintAssignable, Category = "QuestOrderComponent")
		FQuestOrderComponentOrderQueueClearedSignature OnOrderQueueCleared;

private:
	UPROPERTY(BlueprintReadOnly, Category = "QuestOrderComponent", meta = (AllowPrivateAccess = true))
		FQuestOrderData CurrentOrder;

	UPROPERTY(BlueprintReadOnly, Category = "QuestOrderComponent", meta = (AllowPrivateAccess = true))
		FQuestOrderData LastOrder;

	UPROPERTY(BlueprintReadOnly, Category = "QuestOrderComponent", meta = (AllowPrivateAccess = true))
		TArray<FQuestOrderData> OrderQueue;

	UPROPERTY()
		TSoftClassPtr<UQuestOrder> StopOrder;

	UQuestSelectableComponent* SelectableComponent;

	/** 
	* Delegate handles registered on the owner's ability system; needed to abort order if tag requirements
	* no longer met.
	*/
	TMap<FGameplayTag, FDelegateHandle> RegisteredOwnerTagEventHandles;

	/**
	* Delegate handles registered on the target's ability system; needed to abort order if tag requirements
	* no longer met.
	*/
	TMap<FGameplayTag, FDelegateHandle> RegisteredTargetTagEventHandles;

	/** Store the character's current order */
	void SetCurrentOrder(FQuestOrderData NewOrder);

	void ObeyOrder(const FQuestOrderData& Order);
	bool CheckOrder(const FQuestOrderData& Order) const;
	void LogOrderErrorMessage(const FString& Message, const FQuestOrderErrorTags& OrderErrorTags) const;

	UFUNCTION()
		void OnOrderEndedCallback(EQuestOrderResult OrderResult);

	void OrderEnded(EQuestOrderResult OrderResult);
	void OrderCancelled();
	void RegisterTagListeners(const FQuestOrderData& Order);
	void UnregisterTagListeners(const FQuestOrderData& Order);

	UFUNCTION()
		void OnTargetTagsChanged(const FGameplayTag Tag, int32 NewCount);

	UFUNCTION()
		void OnOwnerTagsChanged(const FGameplayTag Tag, int32 NewCount);

	void ObeyStopOrder();


	UFUNCTION(Category = QuestOrderComponent, meta = (DisplayName = "On Selected"))
	void OnSelected();

	virtual void UpdateOrderPreviews();

	/** Last order home location if set */
	FVector LastOrderHomeLocation;

	/** Whether LastOrdedrHomeLocation is valid */
	bool bIsHomeLocationSet;
		
};
