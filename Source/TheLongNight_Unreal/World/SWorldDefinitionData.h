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

private:
	UPROPERTY(EditDefaultsOnly, Category = "Survival|World")
	FSWorldTime StartingWorldTime;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|World")
	float StartingWorldTemperatureC = -12.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Time", meta = (ClampMin = "0.01"))
	float GameMinutesPerRealSecond = 4.0f;
};
