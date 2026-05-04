#include "Core/SPlayerController.h"

#include "Character/SCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/SInteractionPromptWidget.h"

ASPlayerController::ASPlayerController()
{
    bShowMouseCursor = false;
}

void ASPlayerController::UpdateInteractionPrompt()
{
    if (!IsValid(InteractionPromptWidget))
    {
        return;
    }

    ASCharacter* ControlledCharacter = Cast<ASCharacter>(GetPawn());
    if (!IsValid(ControlledCharacter))
    {
        InteractionPromptWidget->HidePrompt();
        return;
    }

    FText InteractionText;
    const bool bHasInteractionText = ControlledCharacter->GetFocusedInteractionText(InteractionText);

    if (!bHasInteractionText)
    {
        InteractionPromptWidget->HidePrompt();
        return;
    }

    const FText FinalPromptText = FText::Format(
        FText::FromString(TEXT("[E] {0}")),
        InteractionText
    );

    InteractionPromptWidget->ShowPrompt(FinalPromptText);
}

void ASPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (!IsLocalController())
    {
        return;
    }

    ULocalPlayer* LocalPlayer = GetLocalPlayer();
    if (!IsValid(LocalPlayer))
    {
        UE_LOG(LogTemp, Error, TEXT("BeginPlay failed: LocalPlayer is invalid."));
        return;
    }

    UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

    if (!IsValid(InputSubsystem))
    {
        UE_LOG(LogTemp, Error, TEXT("BeginPlay failed: Enhanced Input subsystem is invalid."));
        return;
    }

    if (!IsValid(GameplayMappingContext))
    {
        UE_LOG(LogTemp, Error, TEXT("BeginPlay failed: GameplayMappingContext is not assigned."));
        return;
    }

    InputSubsystem->AddMappingContext(GameplayMappingContext, GameplayMappingPriority);

    SetInputMode(FInputModeGameOnly());
    bShowMouseCursor = false;

    CreateInteractionPromptWidget();

    GetWorldTimerManager().SetTimer(
        InteractionPromptTimerHandle,
        this,
        &ASPlayerController::UpdateInteractionPrompt,
        InteractionPromptUpdateInterval,
        true
    );

    UE_LOG(LogTemp, Log, TEXT("Gameplay input mapping context added."));
}

void ASPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
    if (!IsValid(EnhancedInputComponent))
    {
        UE_LOG(LogTemp, Error, TEXT("SetupInputComponent failed: InputComponent is not UEnhancedInputComponent."));
        return;
    }

    if (IsValid(MoveAction))
    {
        EnhancedInputComponent->BindAction(
            MoveAction,
            ETriggerEvent::Triggered,
            this,
            &ASPlayerController::Move
        );
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("MoveAction is not assigned."));
    }

    if (IsValid(LookAction))
    {
        EnhancedInputComponent->BindAction(
            LookAction,
            ETriggerEvent::Triggered,
            this,
            &ASPlayerController::Look
        );
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("LookAction is not assigned."));
    }

    if (IsValid(InteractAction))
    {
        EnhancedInputComponent->BindAction(
            InteractAction,
            ETriggerEvent::Started,
            this,
            &ASPlayerController::Interact
        );
    }

    if (IsValid(SprintAction))
    {
        EnhancedInputComponent->BindAction(
            SprintAction,
            ETriggerEvent::Started,
            this,
            &ASPlayerController::StartSprint
        );

        EnhancedInputComponent->BindAction(
            SprintAction,
            ETriggerEvent::Completed,
            this,
            &ASPlayerController::StopSprint
        );
    }

    if (IsValid(PauseAction))
    {
        EnhancedInputComponent->BindAction(
            PauseAction,
            ETriggerEvent::Started,
            this,
            &ASPlayerController::TogglePause
        );
    }
}

void ASPlayerController::Move(const FInputActionValue& Value)
{
    ASCharacter* ControlledCharacter = Cast<ASCharacter>(GetPawn());
    if (!IsValid(ControlledCharacter))
    {
        UE_LOG(LogTemp, Warning, TEXT("Move failed: controlled pawn is not ASCharacter."));
        return;
    }

    const FVector2D MoveVector = Value.Get<FVector2D>();

    ControlledCharacter->Move(MoveVector);
}

void ASPlayerController::Look(const FInputActionValue& Value)
{
    ASCharacter* ControlledCharacter = Cast<ASCharacter>(GetPawn());
    if (!IsValid(ControlledCharacter))
    {
        UE_LOG(LogTemp, Warning, TEXT("Look failed: controlled pawn is not ASCharacter."));
        return;
    }

    const FVector2D LookVector = Value.Get<FVector2D>();

    ControlledCharacter->Look(LookVector);
}

void ASPlayerController::Interact()
{
    ASCharacter* ControlledCharacter = Cast<ASCharacter>(GetPawn());
    if (!IsValid(ControlledCharacter))
    {
        UE_LOG(LogTemp, Warning, TEXT("Interact failed: controlled pawn is not ASCharacter."));
        return;
    }

    ControlledCharacter->TryInteract();

    UpdateInteractionPrompt();
}

void ASPlayerController::StartSprint()
{
    ASCharacter* ControlledCharacter = Cast<ASCharacter>(GetPawn());
    if (!IsValid(ControlledCharacter))
    {
        UE_LOG(LogTemp, Warning, TEXT("StartSprint failed: controlled pawn is not ASCharacter."));
        return;
    }

    ControlledCharacter->StartSprint();
}

void ASPlayerController::StopSprint()
{
    ASCharacter* ControlledCharacter = Cast<ASCharacter>(GetPawn());
    if (!IsValid(ControlledCharacter))
    {
        UE_LOG(LogTemp, Warning, TEXT("StopSprint failed: controlled pawn is not ASCharacter."));
        return;
    }

    ControlledCharacter->StopSprint();
}

void ASPlayerController::TogglePause()
{
    const bool bIsPaused = UGameplayStatics::IsGamePaused(this);

    SetPause(!bIsPaused);

    if (bIsPaused)
    {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;

        UE_LOG(LogTemp, Log, TEXT("Game unpaused."));
    }
    else
    {
        FInputModeGameAndUI InputMode;
        InputMode.SetHideCursorDuringCapture(false);

        SetInputMode(InputMode);
        bShowMouseCursor = true;

        UE_LOG(LogTemp, Log, TEXT("Game paused."));
    }
}

void ASPlayerController::CreateInteractionPromptWidget()
{
    if (!IsValid(InteractionPromptWidgetClass))
    {
        UE_LOG(LogTemp, Warning, TEXT("InteractionPromptWidgetClass is not assigned."));
        return;
    }

    InteractionPromptWidget = CreateWidget<USInteractionPromptWidget>(
        this,
        InteractionPromptWidgetClass
    );

    if (!IsValid(InteractionPromptWidget))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create InteractionPromptWidget."));
        return;
    }

    InteractionPromptWidget->AddToViewport();
    InteractionPromptWidget->HidePrompt();
}
