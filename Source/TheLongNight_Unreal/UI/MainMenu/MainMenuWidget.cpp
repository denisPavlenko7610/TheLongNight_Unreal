#include "MainMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (NewButton)
		NewButton->OnClicked.AddDynamic(this, &UMainMenuWidget::NewGame);

	if (LoadButton)
		LoadButton->OnClicked.AddDynamic(this, &UMainMenuWidget::LoadGame);

	if (OptionsButton)
		OptionsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::Options);

	if (ExitButton)
		ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::Exit);
}

void UMainMenuWidget::NewGame()
{
	UGameplayStatics::OpenLevel(this, "Game");
	RemoveFromParent();
}

void UMainMenuWidget::LoadGame()
{
	UE_LOG(LogTemp, Log, TEXT("Load Game Clicked"));
}

void UMainMenuWidget::Options()
{
	UE_LOG(LogTemp, Log, TEXT("Options Clicked"));
}

void UMainMenuWidget::Exit()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
