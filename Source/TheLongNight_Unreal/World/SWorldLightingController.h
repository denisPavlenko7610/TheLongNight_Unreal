#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWorldLightingController.generated.h"

class ADirectionalLight;
class ASkyLight;
class UDirectionalLightComponent;
class UHierarchicalInstancedStaticMeshComponent;
class UMaterialInstanceDynamic;
class UMaterialInterface;
class USceneComponent;
class USkyLightComponent;
class UStaticMesh;
class UStaticMeshComponent;

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
	void AutoBindLightingActors();
	void DisableCompetingDirectionalLights() const;
	void LoadGeneratedAssets();
	void BuildStars();
	void UpdateLighting(float DeltaSeconds);
	float GetVisualMinutesSinceMidnight(float SourceMinutesSinceMidnight);
	void UpdateSkyVisuals(float DayAlpha, float NightAlpha);
	FVector GetViewerLocation() const;
	float GetDayAlpha(float MinutesSinceMidnight) const;
	float GetSunHeight(float DayAlpha) const;
	float GetSunVisibilityAlpha(float Height) const;

private:
	UPROPERTY(VisibleAnywhere, Category = "Survival|Lighting")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Survival|Lighting")
	TObjectPtr<USkyLightComponent> GeneratedSkyLightComponent;

	UPROPERTY(VisibleAnywhere, Category = "Survival|Lighting")
	TObjectPtr<UDirectionalLightComponent> MoonLightComponent;

	UPROPERTY(VisibleAnywhere, Category = "Survival|Lighting")
	TObjectPtr<UStaticMeshComponent> MoonMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Survival|Lighting")
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> StarsComponent;

	UPROPERTY(EditInstanceOnly, Category = "Survival|Lighting")
	TObjectPtr<ADirectionalLight> SunLight;

	UPROPERTY(EditInstanceOnly, Category = "Survival|Lighting")
	TObjectPtr<ASkyLight> SkyLight;

	UPROPERTY(EditAnywhere, Category = "Survival|Lighting")
	float SunYawOffset = -45.0f;

	UPROPERTY(EditAnywhere, Category = "Survival|Lighting", meta = (ClampMin = "0.0"))
	float DaySunIntensity = 8.0f;

	UPROPERTY(EditAnywhere, Category = "Survival|Lighting", meta = (ClampMin = "0.0"))
	float NightMoonIntensity = 0.4f;

	UPROPERTY(EditAnywhere, Category = "Survival|Lighting", meta = (ClampMin = "0.0"))
	float DaySkyIntensity = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Survival|Lighting", meta = (ClampMin = "0.0"))
	float NightSkyIntensity = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Survival|Lighting", meta = (ClampMin = "0.01", ClampMax = "1.0"))
	float VisualTimeScale = 0.25f;

	UPROPERTY(EditAnywhere, Category = "Survival|Lighting", meta = (ClampMin = "1000.0"))
	float MoonDistance = 85000.0f;

	UPROPERTY(EditAnywhere, Category = "Survival|Lighting", meta = (ClampMin = "1.0"))
	float MoonVisualScale = 14.0f;

	UPROPERTY(Transient)
	TObjectPtr<UStaticMesh> SphereMesh;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInterface> EmissiveMaterial;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> MoonMaterialInstance;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> StarMaterialInstance;

	bool bStarsBuilt = false;
	bool bHasVisualTime = false;
	float LightingActorRefreshTimer = 0.0f;
	float LastSourceMinutesSinceMidnight = 0.0f;
	float VisualMinutesSinceMidnight = 0.0f;
};
