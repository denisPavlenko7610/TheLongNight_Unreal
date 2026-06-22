#include "World/SWorldLightingController.h"

#include "Components/DirectionalLightComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/LightComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/ASGameState.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

namespace
{
	constexpr float MinutesInDay = 24.0f * 60.0f;
	constexpr int32 StarCount = 900;
	constexpr float StarDistance = 30000.0f;

	const FLinearColor DaySunColor(0.76f, 0.86f, 1.0f, 1.0f);
	const FLinearColor NightMoonColor(0.34f, 0.48f, 0.95f, 1.0f);
	const FLinearColor DaySkyColor(0.70f, 0.82f, 1.0f, 1.0f);
	const FLinearColor NightSkyColor(0.16f, 0.25f, 0.50f, 1.0f);

	float SmoothRange(float From, float To, float Value)
	{
		const float Alpha = FMath::Clamp((Value - From) / (To - From), 0.0f, 1.0f);
		return Alpha * Alpha * (3.0f - 2.0f * Alpha);
	}
}

ASWorldLightingController::ASWorldLightingController()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	GeneratedSkyLightComponent = CreateDefaultSubobject<USkyLightComponent>(TEXT("GeneratedSkyLight"));
	GeneratedSkyLightComponent->SetupAttachment(SceneRoot);
	GeneratedSkyLightComponent->SetMobility(EComponentMobility::Movable);

	MoonLightComponent = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("GeneratedMoonLight"));
	MoonLightComponent->SetupAttachment(SceneRoot);
	MoonLightComponent->SetMobility(EComponentMobility::Movable);
	MoonLightComponent->SetAtmosphereSunLight(false);
	MoonLightComponent->SetForwardShadingPriority(0);
	MoonLightComponent->SetIntensity(0.0f);

	MoonMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GeneratedMoon"));
	MoonMeshComponent->SetupAttachment(SceneRoot);
	MoonMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MoonMeshComponent->SetGenerateOverlapEvents(false);
	MoonMeshComponent->SetCastShadow(false);
	MoonMeshComponent->SetReceivesDecals(false);
	MoonMeshComponent->SetHiddenInGame(true);

	StarsComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("GeneratedStars"));
	StarsComponent->SetupAttachment(SceneRoot);
	StarsComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StarsComponent->SetGenerateOverlapEvents(false);
	StarsComponent->SetCastShadow(false);
	StarsComponent->SetReceivesDecals(false);
	StarsComponent->SetCullDistances(0, 0);
	StarsComponent->SetHiddenInGame(true);
}

void ASWorldLightingController::BeginPlay()
{
	Super::BeginPlay();

	LoadGeneratedAssets();
	AutoBindLightingActors();
	BuildStars();

	if (const UWorld* World = GetWorld())
	{
		if (ASGameState* SGameState = World->GetGameState<ASGameState>())
		{
			AddTickPrerequisiteActor(SGameState);
		}
	}

	UpdateLighting(0.0f);
}

void ASWorldLightingController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	LightingActorRefreshTimer += DeltaSeconds;
	if (LightingActorRefreshTimer >= 0.5f)
	{
		LightingActorRefreshTimer = 0.0f;
		AutoBindLightingActors();
	}

	UpdateLighting(DeltaSeconds);
}

void ASWorldLightingController::AutoBindLightingActors()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	ADirectionalLight* BestDirectionalLight = nullptr;
	float BestIntensity = -1.0f;
	for (TActorIterator<ADirectionalLight> It(World); It; ++It)
	{
		ADirectionalLight* DirectionalLight = *It;
		if (!IsValid(DirectionalLight))
		{
			continue;
		}

		if (UDirectionalLightComponent* LightComponent = Cast<UDirectionalLightComponent>(DirectionalLight->GetLightComponent()))
		{
			LightComponent->SetForwardShadingPriority(0);
			if (LightComponent->Intensity > BestIntensity)
			{
				BestIntensity = LightComponent->Intensity;
				BestDirectionalLight = DirectionalLight;
			}
		}
	}

	if (!IsValid(SunLight))
	{
		SunLight = BestDirectionalLight;
	}

	if (IsValid(SunLight))
	{
		SunLight->SetActorHiddenInGame(false);
		if (UDirectionalLightComponent* LightComponent = Cast<UDirectionalLightComponent>(SunLight->GetLightComponent()))
		{
			LightComponent->SetForwardShadingPriority(2);
			LightComponent->SetAtmosphereSunLight(true);
			LightComponent->SetAtmosphereSunLightIndex(0);
			LightComponent->SetVisibility(true, true);
		}
	}

	DisableCompetingDirectionalLights();

	if (!IsValid(SkyLight))
	{
		for (TActorIterator<ASkyLight> It(World); It; ++It)
		{
			SkyLight = *It;
			if (IsValid(SkyLight))
			{
				break;
			}
		}
	}
}

void ASWorldLightingController::DisableCompetingDirectionalLights() const
{
	if (!IsValid(SunLight))
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	for (TActorIterator<ADirectionalLight> It(World); It; ++It)
	{
		ADirectionalLight* DirectionalLight = *It;
		if (!IsValid(DirectionalLight))
		{
			continue;
		}

		UDirectionalLightComponent* LightComponent = Cast<UDirectionalLightComponent>(DirectionalLight->GetLightComponent());
		if (!IsValid(LightComponent))
		{
			continue;
		}

		if (DirectionalLight == SunLight)
		{
			DirectionalLight->SetActorHiddenInGame(false);
			LightComponent->SetForwardShadingPriority(2);
			LightComponent->SetAtmosphereSunLight(true);
			LightComponent->SetAtmosphereSunLightIndex(0);
			LightComponent->SetVisibility(true, true);
			continue;
		}

		DirectionalLight->SetActorHiddenInGame(true);
		LightComponent->SetForwardShadingPriority(0);
		LightComponent->SetAtmosphereSunLight(false);
		LightComponent->SetVisibility(false, true);
		LightComponent->SetIntensity(0.0f);
	}
}

void ASWorldLightingController::LoadGeneratedAssets()
{
	SphereMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	EmissiveMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/EngineMaterials/EmissiveMeshMaterial.EmissiveMeshMaterial"));

	if (IsValid(SphereMesh.Get()))
	{
		MoonMeshComponent->SetStaticMesh(SphereMesh.Get());
		StarsComponent->SetStaticMesh(SphereMesh.Get());
	}

	if (IsValid(EmissiveMaterial.Get()))
	{
		MoonMaterialInstance = UMaterialInstanceDynamic::Create(EmissiveMaterial.Get(), this);
		StarMaterialInstance = UMaterialInstanceDynamic::Create(EmissiveMaterial.Get(), this);
	}

	if (IsValid(MoonMaterialInstance.Get()))
	{
		MoonMaterialInstance->SetVectorParameterValue(TEXT("Color"), NightMoonColor * 1.6f);
		MoonMaterialInstance->SetVectorParameterValue(TEXT("EmissiveColor"), NightMoonColor * 1.6f);
		MoonMeshComponent->SetMaterial(0, MoonMaterialInstance.Get());
	}

	if (IsValid(StarMaterialInstance.Get()))
	{
		const FLinearColor StarColor(0.70f, 0.84f, 1.0f, 1.0f);
		StarMaterialInstance->SetVectorParameterValue(TEXT("Color"), StarColor * 10.0f);
		StarMaterialInstance->SetVectorParameterValue(TEXT("EmissiveColor"), StarColor * 10.0f);
		StarsComponent->SetMaterial(0, StarMaterialInstance.Get());
	}
}

void ASWorldLightingController::BuildStars()
{
	if (IsValid(StarsComponent))
	{
		StarsComponent->ClearInstances();
		StarsComponent->SetHiddenInGame(true);
	}

	bStarsBuilt = true;
}

void ASWorldLightingController::UpdateLighting(float DeltaSecond)
{
	const UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	const ASGameState* SGameState = World->GetGameState<ASGameState>();
	if (!IsValid(SGameState))
	{
		return;
	}

	const float MinutesSinceMidnight = GetVisualMinutesSinceMidnight(SGameState->GetPreciseMinutesSinceMidnight());
	const float DayAlpha = GetDayAlpha(MinutesSinceMidnight);
	const float SunHeight = GetSunHeight(DayAlpha);
	const float MoonHeight = -SunHeight;
	const float SunVisibilityAlpha = GetSunVisibilityAlpha(SunHeight);
	const float MoonVisibilityAlpha = GetSunVisibilityAlpha(MoonHeight) * (1.0f - SunVisibilityAlpha);
	const float DirectSunAlpha = SmoothRange(0.03f, 0.45f, SunVisibilityAlpha);

	const float SunYaw = SunYawOffset + DayAlpha * 360.0f;
	const FRotator SunRotation(-SunHeight * 90.0f, SunYaw, 0.0f);

	if (IsValid(SunLight))
	{
		SunLight->SetActorRotation(SunRotation);

		if (ULightComponent* LightComponent = SunLight->GetLightComponent())
		{
			LightComponent->SetIntensity(DaySunIntensity * DirectSunAlpha);
			LightComponent->SetLightColor(DaySunColor);
		}
	}

	if (IsValid(MoonLightComponent))
	{
		MoonLightComponent->SetWorldRotation(FRotator(-MoonHeight * 90.0f, SunYaw + 180.0f, 0.0f));
		MoonLightComponent->SetIntensity(FMath::Max(0.85f, NightMoonIntensity) * SmoothRange(0.05f, 0.7f, MoonVisibilityAlpha));
		MoonLightComponent->SetLightColor(NightMoonColor);
	}

	USkyLightComponent* ActiveSkyLightComponent = IsValid(SkyLight)
		? SkyLight->GetLightComponent()
		: GeneratedSkyLightComponent.Get();

	if (IsValid(SkyLight))
	{
		GeneratedSkyLightComponent->SetIntensity(0.0f);
	}

	if (IsValid(ActiveSkyLightComponent))
	{
		ActiveSkyLightComponent->SetIntensity(FMath::Lerp(FMath::Max(0.8f, NightSkyIntensity), DaySkyIntensity, SunVisibilityAlpha));
		ActiveSkyLightComponent->SetLightColor(FLinearColor::LerpUsingHSV(NightSkyColor, DaySkyColor, SunVisibilityAlpha));
	}

	UpdateSkyVisuals(DayAlpha, 1.0f - SunVisibilityAlpha);
}

float ASWorldLightingController::GetVisualMinutesSinceMidnight(float SourceMinutesSinceMidnight)
{
	if (!bHasVisualTime)
	{
		bHasVisualTime = true;
		LastSourceMinutesSinceMidnight = SourceMinutesSinceMidnight;
		VisualMinutesSinceMidnight = SourceMinutesSinceMidnight;
		return VisualMinutesSinceMidnight;
	}

	float SourceDelta = SourceMinutesSinceMidnight - LastSourceMinutesSinceMidnight;
	if (SourceDelta > MinutesInDay * 0.5f)
	{
		SourceDelta -= MinutesInDay;
	}
	else if (SourceDelta < -MinutesInDay * 0.5f)
	{
		SourceDelta += MinutesInDay;
	}

	LastSourceMinutesSinceMidnight = SourceMinutesSinceMidnight;

	if (FMath::Abs(SourceDelta) > 60.0f)
	{
		VisualMinutesSinceMidnight = SourceMinutesSinceMidnight;
		return VisualMinutesSinceMidnight;
	}

	VisualMinutesSinceMidnight = FMath::Fmod(
		VisualMinutesSinceMidnight + SourceDelta * FMath::Clamp(VisualTimeScale, 0.01f, 1.0f) + MinutesInDay,
		MinutesInDay
	);

	return VisualMinutesSinceMidnight;
}

void ASWorldLightingController::UpdateSkyVisuals(float DayAlpha, float NightAlpha)
{
	const FVector ViewerLocation = GetViewerLocation();
	const float SunYaw = SunYawOffset + DayAlpha * 360.0f;
	const float MoonHeight = -GetSunHeight(DayAlpha);
	const float MoonVisibilityAlpha = GetSunVisibilityAlpha(MoonHeight) * NightAlpha;
	const FVector MoonDirection = FRotationMatrix(FRotator(MoonHeight * 70.0f, SunYaw + 180.0f, 0.0f)).GetUnitAxis(EAxis::X);

	if (IsValid(MoonMeshComponent))
	{
		MoonMeshComponent->SetHiddenInGame(MoonVisibilityAlpha < 0.12f);
		MoonMeshComponent->SetWorldLocation(ViewerLocation + MoonDirection * MoonDistance);
		MoonMeshComponent->SetWorldScale3D(FVector(MoonVisualScale));
	}

	if (IsValid(StarsComponent))
	{
		StarsComponent->SetHiddenInGame(true);
	}
}

FVector ASWorldLightingController::GetViewerLocation() const
{
	if (const UWorld* World = GetWorld())
	{
		if (const APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			FVector ViewLocation = FVector::ZeroVector;
			FRotator ViewRotation = FRotator::ZeroRotator;
			PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);
			return ViewLocation;
		}
	}

	return GetActorLocation();
}

float ASWorldLightingController::GetDayAlpha(float MinutesSinceMidnight) const
{
	return FMath::Clamp(MinutesSinceMidnight / MinutesInDay, 0.0f, 1.0f);
}

float ASWorldLightingController::GetSunHeight(float DayAlpha) const
{
	return FMath::Sin(DayAlpha * 2.0f * PI - HALF_PI);
}

float ASWorldLightingController::GetSunVisibilityAlpha(float Height) const
{
	return FMath::Clamp((Height + 0.15f) / 1.15f, 0.0f, 1.0f);
}
