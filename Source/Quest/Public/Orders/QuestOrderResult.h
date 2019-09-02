#pragma once

UENUM(BlueprintType)
enum class EQuestOrderResult : uint8
{
	/** The order was fulfilled */
	SUCCEEDED,

	/** The order was cancelled */
	CANCELLED,

	/** The order failed */
	FAILED,
};
