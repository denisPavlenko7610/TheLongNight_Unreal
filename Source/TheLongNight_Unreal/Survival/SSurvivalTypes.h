#pragma once

#include "CoreMinimal.h"
#include "SSurvivalTypes.generated.h"

USTRUCT()
struct THELONGNIGHT_UNREAL_API FSSurvivalVitals
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Survival|Vitals", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Condition = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Survival|Vitals", meta = (ClampMin = "0.0"))
	float Calories = 2500.0f;

	UPROPERTY(EditAnywhere, Category = "Survival|Vitals", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Thirst = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Survival|Vitals", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Fatigue = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Survival|Vitals")
	float BodyTemperature = 37.0f;
};

USTRUCT()
struct THELONGNIGHT_UNREAL_API FSSurvivalEnvironment
{
	GENERATED_BODY()

	UPROPERTY()
	float AmbientTemperatureC = 0.0f;

	UPROPERTY()
	float WindChillC = 0.0f;

	UPROPERTY()
	float WarmthBonusC = 0.0f;

	float GetFeelsLikeTemperatureC() const
	{
		return AmbientTemperatureC + WarmthBonusC - WindChillC;
	}
};
