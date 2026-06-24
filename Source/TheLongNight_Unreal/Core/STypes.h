#pragma once

#include "CoreMinimal.h"
#include "STypes.generated.h"

namespace STimeConstants
{
	constexpr int32 MinutesPerHour = 60;
	constexpr int32 HoursPerDay = 24;
	constexpr int32 MinutesPerDay = MinutesPerHour * HoursPerDay; // 1440
	constexpr float MinutesPerHourF = 60.0f;
	constexpr float MinutesPerDayF = 1440.0f;
}

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

		while (Minute >= STimeConstants::MinutesPerHour)
		{
			Minute -= STimeConstants::MinutesPerHour;
			Hour++;
		}

		while (Hour >= STimeConstants::HoursPerDay)
		{
			Hour -= STimeConstants::HoursPerDay;
			Day++;
		}
	}

	int32 GetTotalMinutes() const
	{
		return (Day - 1) * STimeConstants::MinutesPerDay + Hour * STimeConstants::MinutesPerHour + Minute;
	}

	int32 GetMinutesSinceMidnight() const
	{
		return Hour * STimeConstants::MinutesPerHour + Minute;
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
		return DawnStartHour * STimeConstants::MinutesPerHour + DawnStartMinute;
	}

	int32 GetDayStartMinutes() const
	{
		return DayStartHour * STimeConstants::MinutesPerHour + DayStartMinute;
	}

	int32 GetDuskStartMinutes() const
	{
		return DuskStartHour * STimeConstants::MinutesPerHour + DuskStartMinute;
	}

	int32 GetNightStartMinutes() const
	{
		return NightStartHour * STimeConstants::MinutesPerHour + NightStartMinute;
	}
};
