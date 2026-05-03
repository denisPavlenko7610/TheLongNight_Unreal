#pragma once

#include "CoreMinimal.h"
#include "STypes.generated.h"

UENUM()
enum class ESTypes : uint8
{
	NewGame,
	LoadGame
};

UENUM()
enum class ESGamePhase : uint8
{
	None,
	Starting,
	Playing,
	Paused,
	GameOver
};

USTRUCT()
struct THELONGNIGHT_UNREAL_API FSWorldTime
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Day = 1;

	UPROPERTY()
	int32 Hour = 8;

	UPROPERTY()
	int32 Minute = 0;

	void AddMinutes(int32 MinutesToAdd)
	{
		if (MinutesToAdd <= 0)
		{
			return;
		}

		Minute += MinutesToAdd;

		while (Minute >= 60)
		{
			Minute -= 60;
			Hour++;
		}

		while (Hour >= 24)
		{
			Hour -= 24;
			Day++;
		}
	}
};
