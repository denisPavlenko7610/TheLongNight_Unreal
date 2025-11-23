#pragma once

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Saver.generated.h"

USTRUCT(BlueprintType)
struct FGameTimeDTO
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Day = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Hour = 8;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Minute = 0;
};

USTRUCT(BlueprintType)
struct FPlayerStatsDTO
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Calories = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Thirst = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Fatigue = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 BodyTempTenth = 370; // 37.0C
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Condition = 100;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector Position = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FItemStackDTO
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName Id; // ItemDef Id
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Quantity = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 ConditionPct = 100;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 CaloriesLeft = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 WaterMlLeft = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 DurabilityUsesLeft = 0;
};

USTRUCT(BlueprintType)
struct FInventoryDTO
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FItemStackDTO> Items;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 CarryCapacityGrams = 30000;
};

USTRUCT(BlueprintType)
struct FWorldStateDTO
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString SceneName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 WeatherProfileId = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 SnowDepth = 0;
};

UCLASS()
class THELONGNIGHT_UNREAL_API USaver : public USaveGame
{
    GENERATED_BODY()
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 Version = 1;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 Seed = 0;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FString SaveUtcIso;

    UPROPERTY(EditAnywhere, BlueprintReadWrite) FGameTimeDTO GameTime;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FPlayerStatsDTO Player;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FInventoryDTO Inventory;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FWorldStateDTO World;

    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bDead = false;
};
