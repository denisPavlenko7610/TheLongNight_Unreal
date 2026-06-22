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

UENUM()
enum class ESWorldTimeOfDay : uint8
{
	Night,
	Dawn,
	Day,
	Dusk
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

	int32 GetMinutesSinceMidnight() const
	{
		return Hour * 60 + Minute;
	}
};

USTRUCT()
struct THELONGNIGHT_UNREAL_API FSDayNightSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Survival|Day Night", meta = (ClampMin = "0", ClampMax = "23"))
	int32 DawnStartHour = 5;

	UPROPERTY(EditAnywhere, Category = "Survival|Day Night", meta = (ClampMin = "0", ClampMax = "59"))
	int32 DawnStartMinute = 0;

	UPROPERTY(EditAnywhere, Category = "Survival|Day Night", meta = (ClampMin = "0", ClampMax = "23"))
	int32 DayStartHour = 8;

	UPROPERTY(EditAnywhere, Category = "Survival|Day Night", meta = (ClampMin = "0", ClampMax = "59"))
	int32 DayStartMinute = 0;

	UPROPERTY(EditAnywhere, Category = "Survival|Day Night", meta = (ClampMin = "0", ClampMax = "23"))
	int32 DuskStartHour = 18;

	UPROPERTY(EditAnywhere, Category = "Survival|Day Night", meta = (ClampMin = "0", ClampMax = "59"))
	int32 DuskStartMinute = 0;

	UPROPERTY(EditAnywhere, Category = "Survival|Day Night", meta = (ClampMin = "0", ClampMax = "23"))
	int32 NightStartHour = 21;

	UPROPERTY(EditAnywhere, Category = "Survival|Day Night", meta = (ClampMin = "0", ClampMax = "59"))
	int32 NightStartMinute = 0;

	int32 GetDawnStartMinutes() const
	{
		return DawnStartHour * 60 + DawnStartMinute;
	}

	int32 GetDayStartMinutes() const
	{
		return DayStartHour * 60 + DayStartMinute;
	}

	int32 GetDuskStartMinutes() const
	{
		return DuskStartHour * 60 + DuskStartMinute;
	}

	int32 GetNightStartMinutes() const
	{
		return NightStartHour * 60 + NightStartMinute;
	}
};
