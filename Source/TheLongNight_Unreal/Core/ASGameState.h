#pragma once

#include "CoreMinimal.h"
#include "Core/STypes.h"
#include "GameFramework/GameStateBase.h"
#include "ASGameState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FSOnGameMinutePassed, float);
DECLARE_MULTICAST_DELEGATE(FSOnGameHourPassed);
DECLARE_MULTICAST_DELEGATE(FSOnGameDayPassed);
DECLARE_MULTICAST_DELEGATE_OneParam(FSOnWorldTimeOfDayChanged, ESWorldTimeOfDay);

UCLASS()
class THELONGNIGHT_UNREAL_API ASGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ASGameState();

	virtual void Tick(float DeltaSeconds) override;

	ESWorldTimeOfDay GetWorldTimeOfDay() const;

	void InitializeWorldState(
		const FSWorldTime& NewWorldTime,
		float NewWorldTemperature,
		float NewGameMinutesPerRealSecond,
		const FSDayNightSettings& NewDayNightSettings
	);

	void SetDayNightSettings(const FSDayNightSettings& NewDayNightSettings);

	ESGamePhase GetGamePhase() const;
	const FSWorldTime& GetWorldTime() const;
	float GetWorldTemperature() const;

	void SetGamePhase(ESGamePhase NewGamePhase);
	void SetWorldTemperature(float NewWorldTemperature);

	void SetWorldTime(const FSWorldTime& NewWorldTime);
	void SetGameMinutesPerRealSecond(float NewGameMinutesPerRealSecond);

	FSOnGameMinutePassed OnGameMinutePassed;
	FSOnGameHourPassed OnGameHourPassed;
	FSOnGameDayPassed OnGameDayPassed;
	FSOnWorldTimeOfDayChanged OnWorldTimeOfDayChanged;

	void AdvanceWorldTime(float RealDeltaSeconds);

private:
	UPROPERTY()
	ESGamePhase GamePhase = ESGamePhase::None;

	UPROPERTY()
	FSWorldTime WorldTime;

	UPROPERTY()
	float WorldTemperature = -10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Time")
	float GameMinutesPerRealSecond = 4.0f;

	float TimeAccumulator = 0.0f;

	UPROPERTY()
	ESWorldTimeOfDay WorldTimeOfDay = ESWorldTimeOfDay::Day;

	UPROPERTY()
	FSDayNightSettings DayNightSettings;

	void BroadcastTimeEvents(int32 PreviousTotalMinutes, int32 NewTotalMinutes);
	void RefreshWorldTimeOfDay();
	ESWorldTimeOfDay DetermineWorldTimeOfDay(const FSWorldTime& Time) const;
};
