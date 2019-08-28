// Copyright 2019 Kelby Van Patten, All Rights Reserved

#pragma once

/** 
*	Defines how this order interacts with other orders
*/

UENUM(BlueprintType)
enum class EQuestOrderCancellationPolicy : uint8 
{
	/**
	*  Order is instant; does not affect the current order
	*/
	INSTANT,

	/**
	*  Order has duration, and will be cancelled if another order given while this order is pending
	*/
	CAN_BE_CANCELLED,

	/**
	*	Order has a duration.  If another order is issued, the new order will be set as the next order
	*	to be issued after this order is completed.
	*/
	CANNOT_BE_CANCELLED,
};