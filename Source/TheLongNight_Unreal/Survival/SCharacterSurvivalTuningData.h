#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Survival/SSurvivalTypes.h"
#include "SCharacterSurvivalTuningData.generated.h"

USTRUCT()
struct THELONGNIGHT_UNREAL_API FSSurvivalNeedDrainSettings
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, Category = "Survival|Needs")
    float CaloriesDrainPerGameMinute = 1.2f;

    UPROPERTY(EditDefaultsOnly, Category = "Survival|Needs")
    float ThirstDrainPerGameMinute = 0.04f;

    UPROPERTY(EditDefaultsOnly, Category = "Survival|Needs")
    float FatigueDrainPerGameMinute = 0.03f;
};

USTRUCT()
struct THELONGNIGHT_UNREAL_API FSSurvivalConditionDamageSettings
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, Category = "Survival|Damage")
    float StarvingConditionDamagePerGameMinute = 0.02f;

    UPROPERTY(EditDefaultsOnly, Category = "Survival|Damage")
    float DehydratedConditionDamagePerGameMinute = 0.04f;

    UPROPERTY(EditDefaultsOnly, Category = "Survival|Damage")
    float FreezingConditionDamagePerGameMinute = 0.06f;

    UPROPERTY(EditDefaultsOnly, Category = "Survival|Damage")
    float ExhaustedConditionDamagePerGameMinute = 0.01f;
};

USTRUCT()
struct THELONGNIGHT_UNREAL_API FSSurvivalTemperatureSettings
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, Category = "Survival|Temperature")
    float FreezingTemperatureThreshold = 35.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Survival|Temperature")
    float ComfortableTemperatureC = 0.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Survival|Temperature")
    float BodyTemperatureLossPerGameMinuteAtExtremeCold = 0.03f;

    UPROPERTY(EditDefaultsOnly, Category = "Survival|Temperature")
    float BodyTemperatureRecoveryPerGameMinute = 0.02f;

    UPROPERTY(EditDefaultsOnly, Category = "Survival|Temperature")
    float ExtremeColdTemperatureC = -30.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Survival|Temperature")
    float NormalBodyTemperatureC = 37.0f;
};

UCLASS()
class THELONGNIGHT_UNREAL_API USCharacterSurvivalTuningData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Survival")
    FSSurvivalVitals InitialVitals;
    
    UPROPERTY(EditDefaultsOnly, Category = "Survival")
    FSSurvivalNeedDrainSettings NeedDrain;

    UPROPERTY(EditDefaultsOnly, Category = "Survival")
    FSSurvivalConditionDamageSettings ConditionDamage;

    UPROPERTY(EditDefaultsOnly, Category = "Survival")
    FSSurvivalTemperatureSettings Temperature;
};
