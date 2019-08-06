#pragma once

/**
 * Describes how the order will be handled.
 */

UENUM(BlueprintType)
enum class EQuestOrderProcessPolicy : uint8
{
	/**
	* Instant orders right away and does not change the current order.
	*/
	INSTANT,

	/**
	*	Order has duration and will be cancelled if another order is issued
	*/
	CAN_BE_CANCELLED,

	/**
	*	Order has duration, and will only be cancelled only if tag requirements are not satisfied
	*/
	CANNOT_BE_CANCELLED,
};