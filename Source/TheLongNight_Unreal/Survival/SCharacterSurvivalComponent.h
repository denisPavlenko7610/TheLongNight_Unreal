#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Survival/SCharacterSurvivalTuningData.h"
#include "Survival/SSurvivalTypes.h"
#include "SCharacterSurvivalComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FSOnSurvivalChanged);
DECLARE_MULTICAST_DELEGATE(FSOnCharacterDied);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class THELONGNIGHT_UNREAL_API USCharacterSurvivalComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USCharacterSurvivalComponent();

    virtual void BeginPlay() override;

    void AdvanceSurvival(float GameMinutes, const FSSurvivalEnvironment& Environment);

    float GetCondition() const;
    float GetCalories() const;
    float GetThirst() const;
    float GetFatigue() const;
    float GetBodyTemperature() const;

    bool IsDead() const;
    bool IsStarving() const;
    bool IsDehydrated() const;
    bool IsExhausted() const;
    bool IsFreezing() const;

    const FSSurvivalVitals& GetVitals() const;
    void RestoreVitals(const FSSurvivalVitals& NewVitals, bool bNewDead);

    FSOnSurvivalChanged OnSurvivalChanged;
    FSOnCharacterDied OnCharacterDied;

private:
    void ApplyNeedsDrain(float GameMinutes);
    void ApplyConditionDamage(float GameMinutes);
    void ClampValues();
    void ApplyBodyTemperature(float GameMinutes, const FSSurvivalEnvironment& Environment);

    UPROPERTY(EditDefaultsOnly, Category = "Survival|Tuning")
    TObjectPtr<USCharacterSurvivalTuningData> TuningData;

    UPROPERTY(VisibleInstanceOnly, Category = "Survival|Vitals")
    FSSurvivalVitals Vitals;

    bool bDead = false;
};
