#include "GameTimeSystem.h"
#include "Math/UnrealMathUtility.h"

void UGameTimeSubsystem::ResetTime(int32 Day, int32 Hour, int32 Minute)
{
	Current.Day = FMath::Max(1, Day);
	Current.Hour = FMath::Clamp(Hour, 0, 23);
	Current.Minute = FMath::Clamp(Minute, 0, 59);
	AccumulatedMinutes = 0.0;
}

void UGameTimeSubsystem::AdvanceByRealSeconds(float RealSeconds)
{
	if (bPaused || TimescaleMinutesPerSecond <= 0.f || RealSeconds <= 0.f) {
		return;
	}

	RealSeconds = FMath::Min(RealSeconds, MaxDeltaSeconds);

	AccumulatedMinutes += RealSeconds * TimescaleMinutesPerSecond;
	int32 Ready = static_cast<int32>(FMath::FloorToFloat(AccumulatedMinutes));
	if (Ready <= 0) {
		return;
	}

	int32 ToProcess = FMath::Min(Ready, MaxMinutesPerTick);
	AccumulatedMinutes -= ToProcess;

	for (int32 i = 0; i < ToProcess; ++i)
	{
		int32 PrevHour = Current.Hour;
		Current.AddMinutes(1);
		OnGameMinute.Broadcast(Current);
		OnGameMinuteNative.Broadcast(Current);
		if (Current.Hour != PrevHour)
		{
			OnGameHour.Broadcast(Current);
			OnGameHourNative.Broadcast(Current);
		}
	}
}

void UGameTimeSubsystem::Tick(float DeltaTime)
{
	const float Clamped = FMath::Min(DeltaTime, MaxDeltaSeconds);
	AdvanceByRealSeconds(Clamped);
}
