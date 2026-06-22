#pragma once

#include "CoreMinimal.h"
#include "Core/STypes.h"
#include "Survival/SSurvivalTypes.h"
#include "SSaveTypes.generated.h"

USTRUCT()
struct THELONGNIGHT_UNREAL_API FSInventorySaveEntry
{
	GENERATED_BODY()

	UPROPERTY()
	FName ItemId;

	UPROPERTY()
	int32 Quantity = 1;
};

USTRUCT()
struct THELONGNIGHT_UNREAL_API FSInventorySaveData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FSInventorySaveEntry> Items;
};

USTRUCT()
struct THELONGNIGHT_UNREAL_API FSCharacterSurvivalSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	FSSurvivalVitals Vitals;

	UPROPERTY()
	bool bDead = false;
};

USTRUCT()
struct THELONGNIGHT_UNREAL_API FSWorldSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	FSWorldTime WorldTime;

	UPROPERTY()
	float WorldTemperatureC = 0.0f;
};

USTRUCT()
struct THELONGNIGHT_UNREAL_API FSGameSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	FSWorldSaveData World;

	UPROPERTY()
	FSInventorySaveData Inventory;

	UPROPERTY()
	FSCharacterSurvivalSaveData CharacterSurvival;
};
