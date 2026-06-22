#include "Core/ASGameState.h"

ASGameState::ASGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GamePhase != ESGamePhase::Playing)
	{
		return;
	}

	AdvanceWorldTime(DeltaSeconds);
}

void ASGameState::AdvanceWorldTime(float RealDeltaSeconds)
{
	if (RealDeltaSeconds <= 0.0f)
	{
		return;
	}

	TimeAccumulator += RealDeltaSeconds * GameMinutesPerRealSecond;

	if (TimeAccumulator < 1.0f)
	{
		return;
	}

	const int32 WholeMinutes = FMath::FloorToInt(TimeAccumulator);
	TimeAccumulator -= static_cast<float>(WholeMinutes);

	const int32 PreviousTotalMinutes = WorldTime.GetTotalMinutes();

	WorldTime.AddMinutes(WholeMinutes);

	const int32 NewTotalMinutes = WorldTime.GetTotalMinutes();

	BroadcastTimeEvents(PreviousTotalMinutes, NewTotalMinutes);
}

void ASGameState::InitializeWorldState(const FSWorldTime& NewWorldTime,	float NewWorldTemperature, float NewGameMinutesPerRealSecond)
{
	SetWorldTime(NewWorldTime);
	SetWorldTemperature(NewWorldTemperature);
	SetGameMinutesPerRealSecond(NewGameMinutesPerRealSecond);
}

void ASGameState::SetWorldTime(const FSWorldTime& NewWorldTime)
{
	WorldTime = NewWorldTime;
	TimeAccumulator = 0.0f;

	UE_LOG(LogTemp,	Log, TEXT("World time set: Day %d, %02d:%02d."), WorldTime.Day,	WorldTime.Hour,	WorldTime.Minute);
}

void ASGameState::SetGameMinutesPerRealSecond(float NewGameMinutesPerRealSecond)
{
	if (NewGameMinutesPerRealSecond <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetGameMinutesPerRealSecond ignored: value must be positive."));
		return;
	}

	GameMinutesPerRealSecond = NewGameMinutesPerRealSecond;

	UE_LOG(LogTemp,	Log, TEXT("Game minutes per real second set to %.2f."),	GameMinutesPerRealSecond);
}

void ASGameState::BroadcastTimeEvents(int32 PreviousTotalMinutes, int32 NewTotalMinutes)
{
	if (NewTotalMinutes <= PreviousTotalMinutes)
	{
		return;
	}

	for (int32 Minute = PreviousTotalMinutes + 1; Minute <= NewTotalMinutes; ++Minute)
	{
		OnGameMinutePassed.Broadcast(1.0f);

		if (Minute % 60 == 0)
		{
			OnGameHourPassed.Broadcast();
		}

		if (Minute % (24 * 60) == 0)
		{
			OnGameDayPassed.Broadcast();
		}
	}
}

ESGamePhase ASGameState::GetGamePhase() const
{
	return GamePhase;
}

const FSWorldTime& ASGameState::GetWorldTime() const
{
	return WorldTime;
}

float ASGameState::GetWorldTemperature() const
{
	return WorldTemperature;
}

void ASGameState::SetGamePhase(ESGamePhase NewGamePhase)
{
	if (GamePhase == NewGamePhase)
	{
		return;
	}

	GamePhase = NewGamePhase;

	UE_LOG(LogTemp, Log, TEXT("Game phase changed to %d."), static_cast<int32>(GamePhase));
}

void ASGameState::SetWorldTemperature(float NewWorldTemperature)
{
	WorldTemperature = NewWorldTemperature;

	UE_LOG(LogTemp, Log, TEXT("World temperature changed to %.2f."), WorldTemperature);
}
