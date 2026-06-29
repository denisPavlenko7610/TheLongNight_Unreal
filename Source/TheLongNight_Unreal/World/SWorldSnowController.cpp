#include "World/SWorldSnowController.h"

#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

namespace
{
	constexpr int32 MinSnowParticleCount = 6000;
	constexpr int32 SnowLayerCount = 2;
	constexpr int32 SnowBackfillSeedOffset = 7919;

	constexpr float SnowRadiusMin = 600.0f;
	constexpr float SnowRadiusMax = 6000.0f;
	constexpr float SnowHeightMin = 2200.0f;
	constexpr float SnowHeightMax = 6500.0f;
	constexpr float SnowScaleMin = 0.025f;
	constexpr float SnowScaleMax = 0.14f;

	constexpr float SnowFallSpeedMin = 20.0f;
	constexpr float SnowFallSpeedMax = 320.0f;
	constexpr float SnowRespawnIntervalMin = 1.0f;
	constexpr int32 SnowCullStartDistance = 600;
	constexpr int32 SnowCullEndDistance = 6500;

	const FLinearColor SnowflakeGlow(0.70f, 0.82f, 0.92f, 0.45f);
	const FLinearColor SnowflakeBaseColor(0.86f, 0.93f, 0.98f, 0.72f);
}

ASWorldSnowController::ASWorldSnowController()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	SnowComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("GeneratedSnow"));
	SnowComponent->SetupAttachment(SceneRoot);
	SnowComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SnowComponent->SetGenerateOverlapEvents(false);
	SnowComponent->SetCastShadow(false);
	SnowComponent->SetReceivesDecals(false);
	SnowComponent->SetMobility(EComponentMobility::Movable);
	SnowComponent->SetCanEverAffectNavigation(false);
	SnowComponent->SetCullDistances(SnowCullStartDistance, SnowCullEndDistance);
	SnowComponent->SetHiddenInGame(true);
	SnowComponent->bEnableDensityScaling = true;

	SnowBackfillComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("GeneratedSnowBackfill"));
	SnowBackfillComponent->SetupAttachment(SceneRoot);
	SnowBackfillComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SnowBackfillComponent->SetGenerateOverlapEvents(false);
	SnowBackfillComponent->SetCastShadow(false);
	SnowBackfillComponent->SetReceivesDecals(false);
	SnowBackfillComponent->SetMobility(EComponentMobility::Movable);
	SnowBackfillComponent->SetCanEverAffectNavigation(false);
	SnowBackfillComponent->SetCullDistances(SnowCullStartDistance, SnowCullEndDistance);
	SnowBackfillComponent->SetHiddenInGame(true);
	SnowBackfillComponent->bEnableDensityScaling = true;
}

void ASWorldSnowController::BeginPlay()
{
	Super::BeginPlay();

	bEnableSnow = true;
	LoadAssets();
	BuildSnowField();
	UpdateSnow(0.0f);
}

void ASWorldSnowController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateSnow(DeltaSeconds);
}

void ASWorldSnowController::LoadAssets()
{
	SnowflakeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane.Plane"));
	if (!IsValid(SnowflakeMesh.Get()))
	{
		SnowflakeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	}

	if (IsValid(SnowflakeMesh.Get()))
	{
		SnowComponent->SetStaticMesh(SnowflakeMesh.Get());
		SnowBackfillComponent->SetStaticMesh(SnowflakeMesh.Get());
	}

	UMaterialInterface* ActiveSnowMaterial = SnowMaterial.Get();
	if (!IsValid(ActiveSnowMaterial))
	{
		ActiveSnowMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Art/Textures/Snow/M_Snowflake_Unlit_Inst.M_Snowflake_Unlit_Inst"));
	}
	if (!IsValid(ActiveSnowMaterial))
	{
		ActiveSnowMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Art/Textures/Snow/M_Snowflake_Unlit.M_Snowflake_Unlit"));
	}
	if (!IsValid(ActiveSnowMaterial))
	{
		ActiveSnowMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Art/Textures/Snow/M_Snow_Inst.M_Snow_Inst"));
	}
	if (!IsValid(ActiveSnowMaterial))
	{
		ActiveSnowMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Art/Textures/Snow/M_Snow.M_Snow"));
	}
	if (!IsValid(ActiveSnowMaterial))
	{
		ActiveSnowMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/EngineMaterials/DefaultMaterial.DefaultMaterial"));
	}

	if (IsValid(ActiveSnowMaterial))
	{
		SnowMaterialInstance = UMaterialInstanceDynamic::Create(ActiveSnowMaterial, this);
		SnowBackfillMaterialInstance = UMaterialInstanceDynamic::Create(ActiveSnowMaterial, this);

		if (IsValid(SnowMaterialInstance.Get()))
		{
			SnowMaterialInstance->SetVectorParameterValue(TEXT("EmissiveColor"), SnowflakeGlow);
			SnowMaterialInstance->SetVectorParameterValue(TEXT("BaseColor"), SnowflakeBaseColor);
			SnowMaterialInstance->SetScalarParameterValue(TEXT("Opacity"), 0.46f);
			SnowComponent->SetMaterial(0, SnowMaterialInstance.Get());
		}
		if (IsValid(SnowBackfillMaterialInstance.Get()))
		{
			SnowBackfillMaterialInstance->SetVectorParameterValue(TEXT("EmissiveColor"), SnowflakeGlow * 0.78f);
			SnowBackfillMaterialInstance->SetVectorParameterValue(TEXT("BaseColor"), SnowflakeBaseColor);
			SnowBackfillMaterialInstance->SetScalarParameterValue(TEXT("Opacity"), 0.33f);
			SnowBackfillComponent->SetMaterial(0, SnowBackfillMaterialInstance.Get());
		}
	}
}

void ASWorldSnowController::BuildSnowField()
{
	if (bSnowBuilt || !IsValid(SnowComponent) || !IsValid(SnowBackfillComponent) || !IsValid(SnowflakeMesh.Get()))
	{
		return;
	}

	SnowComponent->ClearInstances();
	SnowBackfillComponent->ClearInstances();

	const int32 ParticleCount = FMath::Max(MinSnowParticleCount, SnowParticleCount);
	const int32 PrimaryParticleCount = ParticleCount / SnowLayerCount;
	BuildSnowLayer(SnowComponent.Get(), PrimaryParticleCount, SnowRandomSeed);
	BuildSnowLayer(SnowBackfillComponent.Get(), ParticleCount - PrimaryParticleCount, SnowRandomSeed + SnowBackfillSeedOffset);

	bSnowBuilt = true;
}

void ASWorldSnowController::BuildSnowLayer(
	UHierarchicalInstancedStaticMeshComponent* SnowLayer,
	int32 ParticleCount,
	int32 Seed
) const
{
	if (!IsValid(SnowLayer))
	{
		return;
	}

	FRandomStream RandomStream(Seed);
	const float EffectiveRadius = FMath::Clamp(SnowRadius, SnowRadiusMin, SnowRadiusMax);
	const float EffectiveHeight = FMath::Clamp(SnowSpawnHeight, SnowHeightMin, SnowHeightMax);
	const float EffectiveMinScale = FMath::Clamp(SnowMinScale, SnowScaleMin, SnowScaleMax);
	const float EffectiveMaxScale = FMath::Clamp(SnowMaxScale, EffectiveMinScale, SnowScaleMax);

	for (int32 ParticleIndex = 0; ParticleIndex < ParticleCount; ++ParticleIndex)
	{
		const float Angle = RandomStream.FRandRange(0.0f, 2.0f * PI);
		const float Radius = EffectiveRadius * FMath::Sqrt(RandomStream.FRand());
		const FVector LocalLocation(
			FMath::Cos(Angle) * Radius,
			FMath::Sin(Angle) * Radius,
			RandomStream.FRandRange(-EffectiveHeight * 0.55f, EffectiveHeight * 0.55f)
		);

		const float BaseScale = RandomStream.FRandRange(EffectiveMinScale, EffectiveMaxScale);
		const FVector Scale(
			BaseScale * RandomStream.FRandRange(0.35f, 0.75f),
			BaseScale * RandomStream.FRandRange(0.85f, 1.80f),
			1.0f
		);
		const FRotator Rotation(
			90.0f + RandomStream.FRandRange(-16.0f, 16.0f),
			RandomStream.FRandRange(0.0f, 360.0f),
			RandomStream.FRandRange(-22.0f, 22.0f)
		);

		SnowLayer->AddInstance(FTransform(Rotation, LocalLocation, Scale));
	}
}

void ASWorldSnowController::UpdateSnow(float DeltaSeconds)
{
	if (!bEnableSnow || !IsValid(SnowComponent) || !IsValid(SnowBackfillComponent))
	{
		if (IsValid(SnowComponent))
		{
			SnowComponent->SetHiddenInGame(true);
		}
		if (IsValid(SnowBackfillComponent))
		{
			SnowBackfillComponent->SetHiddenInGame(true);
		}
		return;
	}

	if (!bSnowBuilt)
	{
		BuildSnowField();
	}

	if (!bSnowBuilt)
	{
		return;
	}

	if (!bHasSnowFieldCenter)
	{
		bHasSnowFieldCenter = true;
		SnowFieldCenter = GetViewerLocation();
	}

	SnowRespawnTimer += DeltaSeconds;
	if (SnowRespawnTimer >= FMath::Max(SnowRespawnIntervalMin, SnowRespawnInterval))
	{
		SnowRespawnTimer = 0.0f;
		SnowFieldCenter = GetViewerLocation();
	}

	const float EffectiveHeight = FMath::Clamp(SnowSpawnHeight, SnowHeightMin, SnowHeightMax);
	const float EffectiveFallSpeed = FMath::Clamp(SnowFallSpeed, SnowFallSpeedMin, SnowFallSpeedMax);
	const float EffectiveRadius = FMath::Clamp(SnowRadius, SnowRadiusMin, SnowRadiusMax);
	SnowFallOffset = FMath::Fmod(SnowFallOffset + EffectiveFallSpeed * DeltaSeconds, EffectiveHeight);
	SnowWindOffset = FMath::Fmod(SnowWindOffset + FMath::Max(0.0f, SnowWindSpeed) * 0.006f * DeltaSeconds, 2.0f * PI);
	SnowSwayTime += DeltaSeconds;

	FVector2D WindDirection = SnowWindDirection.GetSafeNormal();
	if (WindDirection.IsNearlyZero())
	{
		WindDirection = FVector2D(1.0f, 0.0f);
	}

	const FVector Wind(WindDirection.X, WindDirection.Y, 0.0f);
	const FVector SideWind(-Wind.Y, Wind.X, 0.0f);
	const float EffectiveSwayAmplitude = FMath::Clamp(SnowSwayAmplitude, 0.0f, EffectiveRadius * 0.18f);
	const FVector PrimarySway =
		Wind * FMath::Sin(SnowWindOffset) * EffectiveSwayAmplitude * 1.20f
		+ Wind * FMath::Sin(SnowSwayTime * 0.55f) * EffectiveSwayAmplitude
		+ SideWind * FMath::Sin(SnowSwayTime * 0.83f) * EffectiveSwayAmplitude * 0.35f;
	const FVector BackfillSway =
		Wind * FMath::Sin(SnowWindOffset + PI) * EffectiveSwayAmplitude * 0.85f
		+ Wind * FMath::Sin(SnowSwayTime * 0.47f + PI) * EffectiveSwayAmplitude * 0.8f
		+ SideWind * FMath::Sin(SnowSwayTime * 0.71f + HALF_PI) * EffectiveSwayAmplitude * 0.28f;

	const FVector BaseLocation(
		SnowFieldCenter.X,
		SnowFieldCenter.Y,
		SnowFieldCenter.Z + EffectiveHeight * 0.5f
	);

	const float PrimaryOffset = SnowFallOffset;
	const float BackfillOffset = FMath::Fmod(SnowFallOffset + EffectiveHeight * 0.5f, EffectiveHeight);

	SnowComponent->SetHiddenInGame(false);
	SnowBackfillComponent->SetHiddenInGame(false);
	SnowComponent->SetVisibility(true, true);
	SnowBackfillComponent->SetVisibility(true, true);
	SnowComponent->SetWorldLocation(BaseLocation + PrimarySway - FVector(0.0f, 0.0f, PrimaryOffset));
	SnowBackfillComponent->SetWorldLocation(BaseLocation + BackfillSway - FVector(0.0f, 0.0f, BackfillOffset));
}

FVector ASWorldSnowController::GetViewerLocation() const
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
