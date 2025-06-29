#include "PauseMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ResumeButton)
		ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::ResumeGame);

	if (SaveButton)
		SaveButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::SaveGame);

	if (LoadButton)
		LoadButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::LoadGame);

	if (OptionsButton)
		OptionsButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::Options);

	if (ExitButton)
		ExitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::HandleExit);
}

void UPauseMenuWidget::ResumeGame()
{
	OnResumeClickedDelegate.Broadcast();
}

void UPauseMenuWidget::SaveGame()
{
	//UE_LOG(LogTemp, Log, TEXT("Save Game Clicked"));
}

void UPauseMenuWidget::LoadGame()
{
	//UE_LOG(LogTemp, Log, TEXT("Load Game Clicked"));
}

void UPauseMenuWidget::Options()
{
	//UE_LOG(LogTemp, Log, TEXT("Options Clicked"));
}

void UPauseMenuWidget::HandleExit()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
