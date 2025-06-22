// Copyright Epic Games, Inc. All Rights Reserved.

#include "LevelGameMode.h"

#include "Kismet/GameplayStatics.h"

void ALevelGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->SetShowMouseCursor(false);
	PlayerController->SetInputMode(FInputModeGameOnly());
}
