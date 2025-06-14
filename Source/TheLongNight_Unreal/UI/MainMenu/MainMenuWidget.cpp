#include "MainMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/MainHUD.h"

void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (NewGameButton)
		NewGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleNewGame);

	if (LoadGameButton)
		LoadGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleLoadGame);

	if (OptionsButton)
		OptionsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleOptions);

	if (ExitButton)
		ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleExit);
}

void UMainMenuWidget::HandleNewGame()
{
	UGameplayStatics::OpenLevel(this, "Game");
	RemoveFromParent();
}

void UMainMenuWidget::HandleLoadGame()
{
	UE_LOG(LogTemp, Log, TEXT("Load Game Clicked"));
}

void UMainMenuWidget::HandleOptions()
{
	UE_LOG(LogTemp, Log, TEXT("Options Clicked"));
}

void UMainMenuWidget::HandleExit()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
