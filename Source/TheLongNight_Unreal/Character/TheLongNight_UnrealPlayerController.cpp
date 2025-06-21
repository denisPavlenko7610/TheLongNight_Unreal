// Copyright Epic Games, Inc. All Rights Reserved.


#include "TheLongNight_UnrealPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "TheLongNight_UnrealCameraManager.h"

ATheLongNight_UnrealPlayerController::ATheLongNight_UnrealPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = ATheLongNight_UnrealCameraManager::StaticClass();
}

void ATheLongNight_UnrealPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}
