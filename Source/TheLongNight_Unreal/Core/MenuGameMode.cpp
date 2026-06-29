#include "Core/MenuGameMode.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

AMenuGameMode::AMenuGameMode()
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = APlayerController::StaticClass();
}

void AMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(MainMenuWidgetClass))
	{
		MainMenuWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/BP_MainMenu.BP_MainMenu_C"));
		if (!IsValid(MainMenuWidgetClass))
		{
			UE_LOG(LogTemp, Error, TEXT("MenuGameMode failed: MainMenuWidgetClass is not assigned."));
			return;
		}
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Error, TEXT("MenuGameMode failed: PlayerController is invalid."));
		return;
	}

	MainMenuWidget = CreateWidget<UUserWidget>(PlayerController, MainMenuWidgetClass);
	if (!IsValid(MainMenuWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("MenuGameMode failed: could not create main menu widget."));
		return;
	}

	MainMenuWidget->AddToViewport();

	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->SetShowMouseCursor(true);
}
