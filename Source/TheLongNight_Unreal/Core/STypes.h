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

	UPROPERTY(EditAnywhere, Category = "Survival|Time", meta = (ClampMin = "1"))
	int32 Day = 1;

	UPROPERTY(EditAnywhere, Category = "Survival|Time", meta = (ClampMin = "0", ClampMax = "23"))
	int32 Hour = 8;

	UPROPERTY(EditAnywhere, Category = "Survival|Time", meta = (ClampMin = "0", ClampMax = "59"))
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

	int32 GetTotalMinutes() const
	{
		return (Day - 1) * 24 * 60 + Hour * 60 + Minute;
	}
};
