#pragma once

UENUM(BlueprintType)
enum class EQuestOrderResult : uint8
{
	/** The order was completed successfully */
	SUCCEEDED,

	/** The order was cancelled */
	CANCELLED,

	/** The order has failed */
	FAILED,
};