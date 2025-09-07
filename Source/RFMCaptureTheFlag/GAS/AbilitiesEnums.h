#pragma once


UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None UMETA(Hidden),
	Confirm,
	Cancel,
	Jump,
	Fire,
};
