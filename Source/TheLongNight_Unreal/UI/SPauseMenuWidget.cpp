#include "UI/SPauseMenuWidget.h"

#include "Components/Button.h"
#include "Core/SPlayerController.h"
#include "UI/OptionsMenuWidget.h"

void USPauseMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!IsValid(ResumeButton) ||
        !IsValid(SaveButton) ||
        !IsValid(LoadButton))
    {
        UE_LOG(LogTemp, Error, TEXT("PauseMenuWidget is missing one or more required buttons."));
        return;
    }

    ResumeButton->OnClicked.AddDynamic(this, &USPauseMenuWidget::HandleResumeClicked);
    SaveButton->OnClicked.AddDynamic(this, &USPauseMenuWidget::HandleSaveClicked);
    LoadButton->OnClicked.AddDynamic(this, &USPauseMenuWidget::HandleLoadClicked);

    if (IsValid(MainMenuButton))
    {
        MainMenuButton->OnClicked.AddDynamic(this, &USPauseMenuWidget::HandleMainMenuClicked);
    }

    if (IsValid(ExitButton))
    {
        ExitButton->OnClicked.AddDynamic(this, &USPauseMenuWidget::HandleMainMenuClicked);
    }

    if (IsValid(OptionsButton))
    {
        OptionsButton->OnClicked.AddDynamic(this, &USPauseMenuWidget::HandleOptionsClicked);
    }
}

void USPauseMenuWidget::NativeDestruct()
{
    if (IsValid(ResumeButton))
    {
        ResumeButton->OnClicked.RemoveDynamic(this, &USPauseMenuWidget::HandleResumeClicked);
    }

    if (IsValid(SaveButton))
    {
        SaveButton->OnClicked.RemoveDynamic(this, &USPauseMenuWidget::HandleSaveClicked);
    }

    if (IsValid(LoadButton))
    {
        LoadButton->OnClicked.RemoveDynamic(this, &USPauseMenuWidget::HandleLoadClicked);
    }

    if (IsValid(MainMenuButton))
    {
        MainMenuButton->OnClicked.RemoveDynamic(this, &USPauseMenuWidget::HandleMainMenuClicked);
    }

    if (IsValid(ExitButton))
    {
        ExitButton->OnClicked.RemoveDynamic(this, &USPauseMenuWidget::HandleMainMenuClicked);
    }

    if (IsValid(OptionsButton))
    {
        OptionsButton->OnClicked.RemoveDynamic(this, &USPauseMenuWidget::HandleOptionsClicked);
    }

    Super::NativeDestruct();
}

void USPauseMenuWidget::HandleResumeClicked()
{
    ASPlayerController* SPlayerController = Cast<ASPlayerController>(GetOwningPlayer());
    if (!IsValid(SPlayerController))
    {
        UE_LOG(LogTemp, Error, TEXT("HandleResumeClicked failed: owning player is not ASPlayerController."));
        return;
    }

    SPlayerController->ResumeGame();
}

void USPauseMenuWidget::HandleSaveClicked()
{
    ASPlayerController* SPlayerController = Cast<ASPlayerController>(GetOwningPlayer());
    if (!IsValid(SPlayerController))
    {
        UE_LOG(LogTemp, Error, TEXT("HandleSaveClicked failed: owning player is not ASPlayerController."));
        return;
    }

    SPlayerController->RequestSaveGame();
}

void USPauseMenuWidget::HandleLoadClicked()
{
    ASPlayerController* SPlayerController = Cast<ASPlayerController>(GetOwningPlayer());
    if (!IsValid(SPlayerController))
    {
        UE_LOG(LogTemp, Error, TEXT("HandleLoadClicked failed: owning player is not ASPlayerController."));
        return;
    }

    SPlayerController->RequestLoadGame();
}

void USPauseMenuWidget::HandleMainMenuClicked()
{
    ASPlayerController* SPlayerController = Cast<ASPlayerController>(GetOwningPlayer());
    if (!IsValid(SPlayerController))
    {
        UE_LOG(LogTemp, Error, TEXT("HandleMainMenuClicked failed: owning player is not ASPlayerController."));
        return;
    }

    SPlayerController->RequestReturnToMainMenu();
}

void USPauseMenuWidget::HandleOptionsClicked()
{
    CreateOptionsMenuWidget();
    if (!IsValid(OptionsMenuWidget))
    {
        return;
    }

    OptionsMenuWidget->OpenFrom(this);
}

void USPauseMenuWidget::ResumeGame()
{
    HandleResumeClicked();
}

void USPauseMenuWidget::SaveGame()
{
    HandleSaveClicked();
}

void USPauseMenuWidget::LoadGame()
{
    HandleLoadClicked();
}

void USPauseMenuWidget::Options()
{
    HandleOptionsClicked();
}

void USPauseMenuWidget::HandleExit()
{
    HandleMainMenuClicked();
}

void USPauseMenuWidget::CreateOptionsMenuWidget()
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
