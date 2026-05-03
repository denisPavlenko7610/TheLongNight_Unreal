#include "Core/SGameMode.h"
#include "Core/ASGameState.h"

ASGameMode::ASGameMode()
{
	GameStateClass = ASGameState::StaticClass();
}

void ASGameMode::BeginPlay()
{
	Super::BeginPlay();

	StartWorld();
}

void ASGameMode::StartWorld()
{
	SetupNewWorld();

	ASGameState* SGameState = GetGameState<ASGameState>();
	if (!IsValid(SGameState))
	{
		UE_LOG(LogTemp, Error, TEXT("StartWorld failed: ASGameState is invalid."));
		return;
	}

	SGameState->SetGamePhase(ESGamePhase::Playing);

	UE_LOG(LogTemp, Log, TEXT("World started."));
}

void ASGameMode::RequestGameOver()
{
	ASGameState* SGameState = GetGameState<ASGameState>();
	if (!IsValid(SGameState))
	{
		UE_LOG(LogTemp, Error, TEXT("RequestGameOver failed: ASGameState is invalid."));
		return;
	}

	SGameState->SetGamePhase(ESGamePhase::GameOver);

	UE_LOG(LogTemp, Warning, TEXT("Game Over."));
}

void ASGameMode::SetupNewWorld()
{
	ASGameState* SGameState = GetGameState<ASGameState>();
	if (!IsValid(SGameState))
	{
		UE_LOG(LogTemp, Error, TEXT("SetupNewWorld failed: ASGameState is invalid."));
		return;
	}

	SGameState->SetWorldTemperature(-12.0f);
}
