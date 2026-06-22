#include "World/SWorldSnowController.h"

#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

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
	SnowComponent->SetCullDistances(0, 0);
	SnowComponent->SetHiddenInGame(true);

	SnowBackfillComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("GeneratedSnowBackfill"));
	SnowBackfillComponent->SetupAttachment(SceneRoot);
	SnowBackfillComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SnowBackfillComponent->SetGenerateOverlapEvents(false);
	SnowBackfillComponent->SetCastShadow(false);
	SnowBackfillComponent->SetReceivesDecals(false);
	SnowBackfillComponent->SetCullDistances(0, 0);
	SnowBackfillComponent->SetHiddenInGame(true);
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
	SphereMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));

	if (IsValid(SphereMesh.Get()))
	{
		SnowComponent->SetStaticMesh(SphereMesh.Get());
		SnowBackfillComponent->SetStaticMesh(SphereMesh.Get());
	}

	UMaterialInterface* ActiveSnowMaterial = SnowMaterial.Get();
	if (!IsValid(ActiveSnowMaterial))
	{
		ActiveSnowMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Art/Textures/Snow/M_Snow_Inst.M_Snow_Inst"));
	}
	if (!IsValid(ActiveSnowMaterial))
	{
		ActiveSnowMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Art/Textures/Snow/M_Snowflake.M_Snowflake"));
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

		const FLinearColor SubtleGlow(0.25f, 0.25f, 0.30f, 0.5f);

		if (IsValid(SnowMaterialInstance.Get()))
		{
			SnowMaterialInstance->SetVectorParameterValue(TEXT("EmissiveColor"), SubtleGlow);
			SnowComponent->SetMaterial(0, SnowMaterialInstance.Get());
		}
		if (IsValid(SnowBackfillMaterialInstance.Get()))
		{
			SnowBackfillMaterialInstance->SetVectorParameterValue(TEXT("EmissiveColor"), SubtleGlow);
			SnowBackfillComponent->SetMaterial(0, SnowBackfillMaterialInstance.Get());
		}
	}
}

void ASWorldSnowController::BuildSnowField()
{
	if (bSnowBuilt || !IsValid(SnowComponent) || !IsValid(SnowBackfillComponent) || !IsValid(SphereMesh.Get()))
	{
		return;
	}

	SnowComponent->ClearInstances();
	SnowBackfillComponent->ClearInstances();

	const int32 ParticleCount = FMath::Max(20000, SnowParticleCount);
	const int32 PrimaryParticleCount = ParticleCount / 2;
	BuildSnowLayer(SnowComponent.Get(), PrimaryParticleCount, SnowRandomSeed);
	BuildSnowLayer(SnowBackfillComponent.Get(), ParticleCount - PrimaryParticleCount, SnowRandomSeed + 7919);

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
	const float EffectiveRadius = FMath::Clamp(SnowRadius, 600.0f, 6000.0f);
	const float EffectiveHeight = FMath::Clamp(SnowSpawnHeight, 2200.0f, 6500.0f);
	const float EffectiveMinScale = FMath::Clamp(SnowMinScale, 0.020f, 0.12f);
	const float EffectiveMaxScale = FMath::Clamp(SnowMaxScale, EffectiveMinScale, 0.12f);

	for (int32 ParticleIndex = 0; ParticleIndex < ParticleCount; ++ParticleIndex)
	{
		const float Angle = RandomStream.FRandRange(0.0f, 2.0f * PI);
		const float Radius = EffectiveRadius * FMath::Sqrt(RandomStream.FRand());
		const FVector LocalLocation(
			FMath::Cos(Angle) * Radius,
			FMath::Sin(Angle) * Radius,
			RandomStream.FRandRange(-EffectiveHeight * 0.5f, EffectiveHeight * 0.5f)
		);

		const float Scale = RandomStream.FRandRange(EffectiveMinScale, EffectiveMaxScale);
		const FRotator Rotation(0.0f, RandomStream.FRandRange(0.0f, 360.0f), 0.0f);

		SnowLayer->AddInstance(FTransform(Rotation, LocalLocation, FVector(Scale)));
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
	if (SnowRespawnTimer >= FMath::Max(1.0f, SnowRespawnInterval))
	{
		SnowRespawnTimer = 0.0f;
		SnowFieldCenter = GetViewerLocation();
	}

	const float EffectiveHeight = FMath::Clamp(SnowSpawnHeight, 2200.0f, 6500.0f);
	const float EffectiveFallSpeed = FMath::Clamp(SnowFallSpeed, 20.0f, 320.0f);
	SnowFallOffset = FMath::Fmod(SnowFallOffset + EffectiveFallSpeed * DeltaSeconds, EffectiveHeight);

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
	SnowComponent->SetWorldLocation(BaseLocation - FVector(0.0f, 0.0f, PrimaryOffset));
	SnowBackfillComponent->SetWorldLocation(BaseLocation - FVector(0.0f, 0.0f, BackfillOffset));
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
