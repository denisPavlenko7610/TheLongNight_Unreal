#include "Survival/SCharacterSurvivalComponent.h"

USCharacterSurvivalComponent::USCharacterSurvivalComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USCharacterSurvivalComponent::BeginPlay()
{
    Super::BeginPlay();

    if (IsValid(TuningData))
    {
        Vitals = TuningData->InitialVitals;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SurvivalComponent has no TuningData assigned."));
    }

    ClampValues();
}

void USCharacterSurvivalComponent::AdvanceSurvival(float GameMinutes, const FSSurvivalEnvironment& Environment)
{
    if (!IsValid(TuningData))
    {
        return;
    }

    if (bDead)
    {
        return;
    }

    if (GameMinutes <= 0.0f)
    {
        return;
    }

    ApplyNeedsDrain(GameMinutes);
    ApplyBodyTemperature(GameMinutes, Environment);
    ApplyConditionDamage(GameMinutes);
    ClampValues();

    OnSurvivalChanged.Broadcast();

    if (Vitals.Condition <= 0.0f && !bDead)
    {
        bDead = true;
        OnCharacterDied.Broadcast();
    }
}

void USCharacterSurvivalComponent::ApplyBodyTemperature(float GameMinutes, const FSSurvivalEnvironment& Environment)
{
    const FSSurvivalTemperatureSettings& TemperatureSettings = TuningData->Temperature;

    const float FeelsLikeTemperature = Environment.GetFeelsLikeTemperatureC();

    if (FeelsLikeTemperature >= TemperatureSettings.ComfortableTemperatureC)
    {
        Vitals.BodyTemperature += TemperatureSettings.BodyTemperatureRecoveryPerGameMinute * GameMinutes;
        return;
    }

    const float ColdAlpha = FMath::Clamp(
        (TemperatureSettings.ComfortableTemperatureC - FeelsLikeTemperature) /
        FMath::Abs(TemperatureSettings.ExtremeColdTemperatureC - TemperatureSettings.ComfortableTemperatureC),
        0.0f,
        1.0f
    );

    const float TemperatureLoss =
        TemperatureSettings.BodyTemperatureLossPerGameMinuteAtExtremeCold *
        ColdAlpha *
        GameMinutes;

    Vitals.BodyTemperature -= TemperatureLoss;
}

float USCharacterSurvivalComponent::GetCondition() const
{
    return Vitals.Condition;
}

float USCharacterSurvivalComponent::GetCalories() const
{
    return Vitals.Calories;
}

float USCharacterSurvivalComponent::GetThirst() const
{
    return Vitals.Thirst;
}

float USCharacterSurvivalComponent::GetFatigue() const
{
    return Vitals.Fatigue;
}

float USCharacterSurvivalComponent::GetBodyTemperature() const
{
    return Vitals.BodyTemperature;
}

bool USCharacterSurvivalComponent::IsDead() const
{
    return bDead;
}

bool USCharacterSurvivalComponent::IsStarving() const
{
    return Vitals.Calories <= 0.0f;
}

bool USCharacterSurvivalComponent::IsDehydrated() const
{
    return Vitals.Thirst <= 0.0f;
}

bool USCharacterSurvivalComponent::IsExhausted() const
{
    return Vitals.Fatigue <= 0.0f;
}

bool USCharacterSurvivalComponent::IsFreezing() const
{
    if (!IsValid(TuningData))
    {
        return false;
    }

    return Vitals.BodyTemperature < TuningData->Temperature.FreezingTemperatureThreshold;
}

void USCharacterSurvivalComponent::ApplyNeedsDrain(float GameMinutes)
{
    Vitals.Calories -= TuningData->NeedDrain.CaloriesDrainPerGameMinute * GameMinutes;
    Vitals.Thirst -= TuningData->NeedDrain.ThirstDrainPerGameMinute * GameMinutes;
    Vitals.Fatigue -= TuningData->NeedDrain.FatigueDrainPerGameMinute * GameMinutes;
}

void USCharacterSurvivalComponent::ApplyConditionDamage(float GameMinutes)
{
    float Damage = 0.0f;

    if (IsStarving())
    {
        Damage += TuningData->ConditionDamage.StarvingConditionDamagePerGameMinute * GameMinutes;
    }

    if (IsDehydrated())
    {
        Damage += TuningData->ConditionDamage.DehydratedConditionDamagePerGameMinute * GameMinutes;
    }

    if (IsFreezing())
    {
        Damage += TuningData->ConditionDamage.FreezingConditionDamagePerGameMinute * GameMinutes;
    }

    if (IsExhausted())
    {
        Damage += TuningData->ConditionDamage.ExhaustedConditionDamagePerGameMinute * GameMinutes;
    }

    Vitals.Condition -= Damage;
}

void USCharacterSurvivalComponent::ClampValues()
{
    Vitals.Condition = FMath::Clamp(Vitals.Condition, 0.0f, 100.0f);
    Vitals.Calories = FMath::Max(0.0f, Vitals.Calories);
    Vitals.Thirst = FMath::Clamp(Vitals.Thirst, 0.0f, 100.0f);
    Vitals.Fatigue = FMath::Clamp(Vitals.Fatigue, 0.0f, 100.0f);
    if (IsValid(TuningData))
    {
        Vitals.BodyTemperature = FMath::Min(Vitals.BodyTemperature, TuningData->Temperature.NormalBodyTemperatureC);
    }
}

const FSSurvivalVitals& USCharacterSurvivalComponent::GetVitals() const
{
    return Vitals;
}

void USCharacterSurvivalComponent::RestoreVitals(
    const FSSurvivalVitals& NewVitals,
    bool bNewDead
)
{
    Vitals = NewVitals;
    bDead = bNewDead;

    ClampValues();

    OnSurvivalChanged.Broadcast();

    if (bDead)
    {
        OnCharacterDied.Broadcast();
    }
}
