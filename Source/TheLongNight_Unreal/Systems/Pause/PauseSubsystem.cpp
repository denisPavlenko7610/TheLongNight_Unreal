#include "PauseSubsystem.h"

#include "Core/SurviveGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "UI/PauseMenu/PauseMenuWidget.h"

void UPauseSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (USurviveGameInstance* GameInstance = Cast<USurviveGameInstance>(GetGameInstance()))
    {
        PauseMenuClass = GameInstance->PauseMenuClass;
    }
    PauseMenuInstance = nullptr;
}

APlayerController* UPauseSubsystem::GetFirstPlayerController() const
{
    if (UWorld* World = GetWorld())
    {
        return UGameplayStatics::GetPlayerController(World, 0);
    }
    return nullptr;
}

void UPauseSubsystem::TogglePause()
{
    UWorld* World = GetWorld();
    if (!World)
        return;

    bool isPaused = !World->IsPaused();
    UGameplayStatics::SetGamePaused(World, isPaused);

    APlayerController* PlayerController = GetFirstPlayerController();
    if (!PlayerController)
        return;

    if (isPaused)
    {
        if (!PauseMenuInstance && PauseMenuClass)
        {
            PauseMenuInstance = CreateWidget<UUserWidget>(PlayerController, PauseMenuClass);
            
            if (UPauseMenuWidget* PauseMenuWidget = Cast<UPauseMenuWidget>(PauseMenuInstance))
            {
                PauseMenuWidget->OnResumeClickedDelegate.AddDynamic(this, &UPauseSubsystem::TogglePause);
            }
        }

        if (PauseMenuInstance && !PauseMenuInstance->IsInViewport())
        {
            PauseMenuInstance->AddToViewport();

            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(PauseMenuInstance->TakeWidget());
            PlayerController->SetInputMode(InputMode);
            PlayerController->bShowMouseCursor = true;
        }
    }
    else
    {
        if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
        {
            PauseMenuInstance->RemoveFromParent();
        }
        
        FInputModeGameOnly GameInput;
        PlayerController->SetInputMode(GameInput);
        PlayerController->bShowMouseCursor = false;
    }
}
