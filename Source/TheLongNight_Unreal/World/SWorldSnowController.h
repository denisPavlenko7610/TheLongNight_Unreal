#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWorldSnowController.generated.h"

class UHierarchicalInstancedStaticMeshComponent;
class UMaterialInstanceDynamic;
class UMaterialInterface;
class USceneComponent;
class UStaticMesh;

UCLASS()
class THELONGNIGHT_UNREAL_API ASWorldSnowController : public AActor
{
	GENERATED_BODY()

public:
	ASWorldSnowController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	void LoadAssets();
	void BuildSnowField();
	void BuildSnowLayer(UHierarchicalInstancedStaticMeshComponent* SnowLayer, int32 ParticleCount, int32 Seed) const;
	void UpdateSnow(float DeltaSeconds);
	FVector GetViewerLocation() const;

	UPROPERTY(VisibleAnywhere, Category = "Survival|Weather")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Survival|Weather")
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> SnowComponent;

	UPROPERTY(VisibleAnywhere, Category = "Survival|Weather")
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> SnowBackfillComponent;

	UPROPERTY(EditAnywhere, Category = "Survival|Weather")
	bool bEnableSnow = true;

	UPROPERTY(EditAnywhere, Category = "Survival|Weather")
	TObjectPtr<UMaterialInterface> SnowMaterial;

	UPROPERTY(EditAnywhere, Category = "Survival|Weather", meta = (ClampMin = "1000"))
	int32 SnowParticleCount = 55000;

	UPROPERTY(EditAnywhere, Category = "Survival|Weather", meta = (ClampMin = "600.0"))
	float SnowRadius = 5000.0f;

	UPROPERTY(EditAnywhere, Category = "Survival|Weather", meta = (ClampMin = "500.0"))
	float SnowSpawnHeight = 3200.0f;

	UPROPERTY(EditAnywhere, Category = "Survival|Weather", meta = (ClampMin = "0.0"))
	float SnowFallSpeed = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Survival|Weather", meta = (ClampMin = "0.001"))
	float SnowMinScale = 0.01f;

	UPROPERTY(EditAnywhere, Category = "Survival|Weather", meta = (ClampMin = "0.001"))
	float SnowMaxScale = 0.02f;

	UPROPERTY(EditAnywhere, Category = "Survival|Weather", meta = (ClampMin = "1.0"))
	float SnowRespawnInterval = 8.0f;

	UPROPERTY(EditAnywhere, Category = "Survival|Weather")
	int32 SnowRandomSeed = 1337;

	UPROPERTY(Transient)
	TObjectPtr<UStaticMesh> SphereMesh;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> SnowMaterialInstance;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> SnowBackfillMaterialInstance;

	bool bSnowBuilt = false;
	bool bHasSnowFieldCenter = false;
	float SnowFallOffset = 0.0f;
	float SnowRespawnTimer = 0.0f;
	FVector SnowFieldCenter = FVector::ZeroVector;
};
