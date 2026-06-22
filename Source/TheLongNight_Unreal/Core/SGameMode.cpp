#include "Core/SGameMode.h"
#include "TimerManager.h"
#include "Core/ASGameState.h"
#include "Core/SGameInstance.h"
#include "Core/SPlayerController.h"
#include "Save/SGameSaveSubsystem.h"
#include "World/SWorldDefinitionData.h"

ASGameMode::ASGameMode() {
	GameStateClass = ASGameState::StaticClass();
	PlayerControllerClass = ASPlayerController::StaticClass();
}

void ASGameMode::BeginPlay() {
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

	if (ShouldLoadGame())
	{
		GetWorldTimerManager().SetTimerForNextTick(
			this,
			&ASGameMode::ApplyPendingLoadIfNeeded
		);
	}

	UE_LOG(LogTemp, Log, TEXT("World started."));
}

bool ASGameMode::ShouldLoadGame() const
{
	const USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
	if (!IsValid(SGameInstance))
	{
		return false;
	}

	return SGameInstance->GetStartGameMode() == ESTypes::LoadGame;
}

void ASGameMode::ApplyPendingLoadIfNeeded()
{
	USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
	if (!IsValid(SGameInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("ApplyPendingLoadIfNeeded failed: USGameInstance is invalid."));
		return;
	}

	if (SGameInstance->GetStartGameMode() != ESTypes::LoadGame)
	{
		return;
	}

	USGameSaveSubsystem* SaveSubsystem = SGameInstance->GetSubsystem<USGameSaveSubsystem>();
	if (!IsValid(SaveSubsystem))
	{
		UE_LOG(LogTemp, Error, TEXT("ApplyPendingLoadIfNeeded failed: SaveSubsystem is invalid."));
		return;
	}

	const FString SlotName = SGameInstance->GetActiveSaveSlotName();

	const bool bLoaded = SaveSubsystem->LoadCurrentGame(SlotName);
	if (!bLoaded)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("ApplyPendingLoadIfNeeded failed to load slot '%s'. New world state will remain active."),
			*SlotName
		);
	}
}

void ASGameMode::RequestGameOver()
{
	ASGameState* SGameState = GetGameState<ASGameState>();
	if (!IsValid(SGameState)) {
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

	if (!IsValid(WorldDefinitionData))
	{
		UE_LOG(LogTemp, Error, TEXT("SetupNewWorld failed: WorldDefinitionData is not assigned."));
		return;
	}

	const bool bRandomizeStartTime = !ShouldLoadGame();

	const FSWorldTime StartingWorldTime =
		WorldDefinitionData->CreateStartingWorldTime(bRandomizeStartTime);

	SGameState->InitializeWorldState(
		StartingWorldTime,
		WorldDefinitionData->GetStartingWorldTemperatureC(),
		WorldDefinitionData->GetGameMinutesPerRealSecond(),
		WorldDefinitionData->GetDayNightSettings()
	);
}
