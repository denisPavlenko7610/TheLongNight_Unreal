// Copyright Epic Games, Inc. All Rights Reserved.


#include "SurvivePlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "TheLongNight_UnrealCameraManager.h"
#include "Systems/PauseSubsystem.h"

ASurvivePlayerController::ASurvivePlayerController()
{
	PlayerCameraManagerClass = ATheLongNight_UnrealCameraManager::StaticClass();
}

void ASurvivePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
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
				EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ASurvivePlayerController::OnPausePressed);
			}
		}
	}
}

void ASurvivePlayerController::OnPausePressed()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UPauseSubsystem* PauseSubsystem = GameInstance->GetSubsystem<UPauseSubsystem>())
		{
			PauseSubsystem->TogglePause();
		}
	}
}
