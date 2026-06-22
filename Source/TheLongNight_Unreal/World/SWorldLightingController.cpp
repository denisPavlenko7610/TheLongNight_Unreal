#include "World/SWorldLightingController.h"

#include "Components/LightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Core/ASGameState.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Engine/World.h"

ASWorldLightingController::ASWorldLightingController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASWorldLightingController::BeginPlay()
{
    Super::BeginPlay();

    UpdateLighting();
}

void ASWorldLightingController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    UpdateLighting();
}

void ASWorldLightingController::UpdateLighting()
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

    const int32 MinutesSinceMidnight = SGameState->GetWorldTime().GetMinutesSinceMidnight();

    const float DayAlpha = GetDayAlpha(MinutesSinceMidnight);
    const float SunHeight = GetSunHeight(DayAlpha);
    const float SunVisibilityAlpha = GetSunVisibilityAlpha(SunHeight);

    const float SunPitch = SunHeight * 90.0f;
    const float SunYaw = SunYawOffset + DayAlpha * 360.0f;

    if (IsValid(SunLight))
    {
        SunLight->SetActorRotation(FRotator(SunPitch, SunYaw, 0.0f));

        if (ULightComponent* SunLightComponent = SunLight->GetLightComponent())
        {
            const float SunIntensity = FMath::Lerp(
                NightSunIntensity,
                DaySunIntensity,
                SunVisibilityAlpha
            );

            SunLightComponent->SetIntensity(SunIntensity);
        }
    }

    if (IsValid(SkyLight))
    {
        if (USkyLightComponent* SkyLightComponent = SkyLight->GetLightComponent())
        {
            const float SkyIntensity = FMath::Lerp(
                NightSkyIntensity,
                DaySkyIntensity,
                SunVisibilityAlpha
            );

            SkyLightComponent->SetIntensity(SkyIntensity);
        }
    }
}

float ASWorldLightingController::GetDayAlpha(int32 MinutesSinceMidnight) const
{
    constexpr float MinutesInDay = 24.0f * 60.0f;

    return FMath::Clamp(
        static_cast<float>(MinutesSinceMidnight) / MinutesInDay,
        0.0f,
        1.0f
    );
}

float ASWorldLightingController::GetSunHeight(float DayAlpha) const
{
    const float SunAngleRadians = DayAlpha * 2.0f * PI - HALF_PI;

    return FMath::Sin(SunAngleRadians);
}

float ASWorldLightingController::GetSunVisibilityAlpha(float SunHeight) const
{
    return FMath::Clamp(
        (SunHeight + 0.15f) / 1.15f,
        0.0f,
        1.0f
    );
}
