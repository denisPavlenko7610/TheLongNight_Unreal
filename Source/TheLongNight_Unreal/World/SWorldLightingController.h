#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWorldLightingController.generated.h"

class ADirectionalLight;
class ASkyLight;

UCLASS()
class THELONGNIGHT_UNREAL_API ASWorldLightingController : public AActor
{
	GENERATED_BODY()

public:
	ASWorldLightingController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	void UpdateLighting();
	float GetDayAlpha(int32 MinutesSinceMidnight) const;
	float GetSunHeight(float DayAlpha) const;
	float GetSunVisibilityAlpha(float SunHeight) const;

private:
	UPROPERTY(EditInstanceOnly, Category = "Survival|Lighting")
	TObjectPtr<ADirectionalLight> SunLight;

	UPROPERTY(EditInstanceOnly, Category = "Survival|Lighting")
	TObjectPtr<ASkyLight> SkyLight;

	UPROPERTY(EditAnywhere, Category = "Survival|Lighting")
	float SunYawOffset = -45.0f;

	UPROPERTY(EditAnywhere, Category = "Survival|Lighting", meta = (ClampMin = "0.0"))
	float DaySunIntensity = 8.0f;

	UPROPERTY(EditAnywhere, Category = "Survival|Lighting", meta = (ClampMin = "0.0"))
	float NightSunIntensity = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Survival|Lighting", meta = (ClampMin = "0.0"))
	float DaySkyIntensity = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Survival|Lighting", meta = (ClampMin = "0.0"))
	float NightSkyIntensity = 0.05f;
};
