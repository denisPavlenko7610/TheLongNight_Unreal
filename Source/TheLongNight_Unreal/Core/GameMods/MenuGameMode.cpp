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
	
	if (UUserWidget* Menu = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass))
	{
		Menu->AddToViewport();
		
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PlayerController->SetShowMouseCursor(true);
		PlayerController->SetInputMode(FInputModeUIOnly());
	}
}
