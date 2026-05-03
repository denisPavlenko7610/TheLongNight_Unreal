// Copyright Epic Games, Inc. All Rights Reserved.


#include "SPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "TheLongNight_UnrealCameraManager.h"
#include "Systems/Pause/PauseSubsystem.h"

ASPlayerController::ASPlayerController(): PauseAction(nullptr)
{
	PlayerCameraManagerClass = ATheLongNight_UnrealCameraManager::StaticClass();
}

void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : PlayerMappingContext)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}

		for (UInputMappingContext* CurrentContext : GameStateMappingContext)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			if (PauseAction)
			{
				EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this,
				                                   &ASPlayerController::OnPausePressed);
			}
		}
	}
}

void ASPlayerController::OnPausePressed()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UPauseSubsystem* PauseSubsystem = GameInstance->GetSubsystem<UPauseSubsystem>())
		{
			PauseSubsystem->TogglePause();
		}
	}
}
