#pragma once

#include "CoreMinimal.h"
#include "Core/STypes.h"
#include "Engine/DataAsset.h"
#include "SWorldDefinitionData.generated.h"

UCLASS()
class THELONGNIGHT_UNREAL_API USWorldDefinitionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	const FSWorldTime& GetStartingWorldTime() const
	{
		return StartingWorldTime;
	}

	float GetStartingWorldTemperatureC() const
	{
		return StartingWorldTemperatureC;
	}

	float GetGameMinutesPerRealSecond() const
	{
		return GameMinutesPerRealSecond;
	}

	FSWorldTime CreateStartingWorldTime(bool bAllowRandomStartTime) const
	{
		if (!bAllowRandomStartTime || !bRandomizeStartingTimeForNewGame)
		{
			return StartingWorldTime;
		}

		const int32 MinMinutes = GetRandomStartMinMinutes();
		const int32 MaxMinutes = GetRandomStartMaxMinutes();

		int32 RandomMinutes = 0;

		if (MinMinutes <= MaxMinutes)
		{
			RandomMinutes = FMath::RandRange(MinMinutes, MaxMinutes);
		}
		else
		{
			const int32 MinutesInDay = 24 * 60;
			const int32 WrappedRangeLength = (MinutesInDay - MinMinutes) + MaxMinutes + 1;
			const int32 RandomOffset = FMath::RandRange(0, WrappedRangeLength - 1);

			RandomMinutes = (MinMinutes + RandomOffset) % MinutesInDay;
		}

		FSWorldTime Result = StartingWorldTime;
		Result.Hour = RandomMinutes / 60;
		Result.Minute = RandomMinutes % 60;

		return Result;
	}

	const FSDayNightSettings& GetDayNightSettings() const
	{
		return DayNightSettings;
	}

private:
	UPROPERTY(EditDefaultsOnly, Category = "Survival|World")
	FSWorldTime StartingWorldTime;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Time")
	bool bRandomizeStartingTimeForNewGame = true;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Time", meta = (ClampMin = "0", ClampMax = "23"))
	int32 RandomStartMinHour = 6;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Time", meta = (ClampMin = "0", ClampMax = "59"))
	int32 RandomStartMinMinute = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Time", meta = (ClampMin = "0", ClampMax = "23"))
	int32 RandomStartMaxHour = 18;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Time", meta = (ClampMin = "0", ClampMax = "59"))
	int32 RandomStartMaxMinute = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Day Night")
	FSDayNightSettings DayNightSettings;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|World")
	float StartingWorldTemperatureC = -12.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Time", meta = (ClampMin = "0.01"))
	float GameMinutesPerRealSecond = 4.0f;

	int32 GetRandomStartMinMinutes() const
	{
		return FMath::Clamp(RandomStartMinHour * 60 + RandomStartMinMinute, 0, 24 * 60 - 1);
	}

	int32 GetRandomStartMaxMinutes() const
	{
		return FMath::Clamp(RandomStartMaxHour * 60 + RandomStartMaxMinute, 0, 24 * 60 - 1);
	}
};
