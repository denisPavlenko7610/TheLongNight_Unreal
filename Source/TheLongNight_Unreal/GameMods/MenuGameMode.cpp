#include "MenuGameMode.h"

#include "Kismet/GameplayStatics.h"

AMenuGameMode::AMenuGameMode()
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = nullptr;
}

void AMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!MainMenuWidgetClass)
	{
		return;
	}
	
	if (UUserWidget* Menu = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass))
	{
		Menu->AddToViewport();
			
		PlayerController->SetShowMouseCursor(true);
		PlayerController->SetInputMode(FInputModeUIOnly());
	}
}
