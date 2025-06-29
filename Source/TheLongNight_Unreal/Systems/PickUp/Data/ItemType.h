#pragma once

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None = 0 UMETA(DisplayName="None"),
	Apple = 1 UMETA(DisplayName="Apple"),
	Soda = 2 UMETA(DisplayName="Soda"),
};
