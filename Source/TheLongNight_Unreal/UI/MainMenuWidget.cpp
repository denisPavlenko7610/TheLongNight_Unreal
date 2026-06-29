#include "UI/MainMenuWidget.h"

#include "Components/Button.h"
#include "Core/SGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/OptionsMenuWidget.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!IsValid(NewButton) ||
		!IsValid(LoadButton) ||
		!IsValid(ExitButton))
	{
		UE_LOG(LogTemp, Error, TEXT("MainMenuWidget is missing one or more required buttons."));
		return;
	}

	NewButton->OnClicked.AddDynamic(this, &UMainMenuWidget::NewGame);
	LoadButton->OnClicked.AddDynamic(this, &UMainMenuWidget::LoadGame);
	ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::Exit);

	if (IsValid(OptionsButton))
	{
		OptionsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::Options);
	}
}

void UMainMenuWidget::NativeDestruct()
{
	if (IsValid(NewButton))
	{
		NewButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::NewGame);
	}

	if (IsValid(LoadButton))
	{
		LoadButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::LoadGame);
	}

	if (IsValid(OptionsButton))
	{
		OptionsButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::Options);
	}

	if (IsValid(ExitButton))
	{
		ExitButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::Exit);
	}

	Super::NativeDestruct();
}

void UMainMenuWidget::NewGame()
{
	USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
	if (!IsValid(SGameInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("NewGame failed: USGameInstance is invalid."));
		return;
	}

	SGameInstance->StartNewGame();
}

void UMainMenuWidget::LoadGame()
{
	USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
	if (!IsValid(SGameInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("LoadGame failed: USGameInstance is invalid."));
		return;
	}

	SGameInstance->LoadGame();
}

void UMainMenuWidget::Options()
{
	CreateOptionsMenuWidget();
	if (!IsValid(OptionsMenuWidget))
	{
		return;
	}

	OptionsMenuWidget->OpenFrom(this);
}

void UMainMenuWidget::Exit()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UMainMenuWidget::CreateOptionsMenuWidget()
{
	if (IsValid(OptionsMenuWidget))
	{
		return;
	}

	if (!IsValid(OptionsMenuWidgetClass))
	{
		OptionsMenuWidgetClass = LoadClass<UOptionsMenuWidget>(nullptr, TEXT("/Game/UI/BP_OptionsMenu.BP_OptionsMenu_C"));
		if (!IsValid(OptionsMenuWidgetClass))
		{
			UE_LOG(LogTemp, Warning, TEXT("OptionsMenuWidgetClass is not assigned."));
			return;
		}
	}

	OptionsMenuWidget = CreateWidget<UOptionsMenuWidget>(GetOwningPlayer(), OptionsMenuWidgetClass);
	if (!IsValid(OptionsMenuWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create OptionsMenuWidget."));
	}
}
