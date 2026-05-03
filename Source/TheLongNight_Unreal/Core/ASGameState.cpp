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

	TimeAccumulator += DeltaSeconds * GameMinutesPerRealSecond;

	if (TimeAccumulator >= 1.0f)
	{
		const int32 WholeMinutes = FMath::FloorToInt(TimeAccumulator);

		WorldTime.AddMinutes(WholeMinutes);

		TimeAccumulator -= static_cast<float>(WholeMinutes);
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
