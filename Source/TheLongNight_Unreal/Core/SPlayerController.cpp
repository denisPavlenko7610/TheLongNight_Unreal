#include "Core/SPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Character/SCharacter.h"
#include "Core/ASGameState.h"
#include "Core/SGameInstance.h"
#include "Inventory/SInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/SInventoryMenuWidget.h"
#include "UI/SInteractionPromptWidget.h"
#include "UI/SPauseMenuWidget.h"

ASPlayerController::ASPlayerController() {
	bShowMouseCursor = false;
}

void ASPlayerController::UpdateInteractionPrompt() {
	if (!IsValid(InteractionPromptWidget)) {
		return;
	}

	ASCharacter* ControlledCharacter = Cast<ASCharacter>(GetPawn());
	if (!IsValid(ControlledCharacter)) {
		InteractionPromptWidget->HidePrompt();
		return;
	}

	FText InteractionText;
	const bool bHasInteractionText = ControlledCharacter->GetFocusedInteractionText(InteractionText);

	if (!bHasInteractionText) {
		InteractionPromptWidget->HidePrompt();
		return;
	}

	const FText FinalPromptText = FText::Format(FText::FromString(TEXT("[E] {0}")), InteractionText);

	InteractionPromptWidget->ShowPrompt(FinalPromptText);
}

void ASPlayerController::BeginPlay() {
	Super::BeginPlay();

	if (!IsLocalController()) {
		return;
	}

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!IsValid(LocalPlayer)) {
		UE_LOG(LogTemp, Error, TEXT("BeginPlay failed: LocalPlayer is invalid."));
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	if (!IsValid(InputSubsystem)) {
		UE_LOG(LogTemp, Error, TEXT("BeginPlay failed: Enhanced Input subsystem is invalid."));
		return;
	}

	if (!IsValid(GameplayMappingContext)) {
		UE_LOG(LogTemp, Error, TEXT("BeginPlay failed: GameplayMappingContext is not assigned."));
		return;
	}

	InputSubsystem->AddMappingContext(GameplayMappingContext, GameplayMappingPriority);

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;

	CreateInteractionPromptWidget();
	CreatePauseMenuWidget();
	CreateInventoryMenuWidget();

	GetWorldTimerManager().SetTimer(
		InteractionPromptTimerHandle,
		this,
		&ASPlayerController::UpdateInteractionPrompt,
		InteractionPromptUpdateInterval,
		true
	);
}

void ASPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!IsValid(EnhancedInputComponent)) {
		UE_LOG(LogTemp, Error, TEXT("SetupInputComponent failed: InputComponent is not UEnhancedInputComponent."));
		return;
	}

	if (IsValid(MoveAction)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASPlayerController::Move);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("MoveAction is not assigned."));
	}

	if (IsValid(LookAction)) {
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASPlayerController::Look);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("LookAction is not assigned."));
	}

	if (IsValid(InteractAction)) {
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ASPlayerController::Interact);
	}

	if (IsValid(SprintAction)) {
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ASPlayerController::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASPlayerController::StopSprint);
	}

	if (IsValid(PauseAction)) {
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ASPlayerController::TogglePause);
	}

	if (!IsValid(ToggleInventoryAction))
	{
		ToggleInventoryAction = LoadObject<UInputAction>(
			nullptr,
			TEXT("/Game/Input/Actions/IA_ToggleInventory.IA_ToggleInventory")
		);
	}

	if (IsValid(ToggleInventoryAction))
	{
		EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &ASPlayerController::ToggleInventory);
	}
}

void ASPlayerController::Move(const FInputActionValue& Value) {
	ASCharacter* ControlledCharacter = Cast<ASCharacter>(GetPawn());
	if (!IsValid(ControlledCharacter)) {
		UE_LOG(LogTemp, Warning, TEXT("Move failed: controlled pawn is not ASCharacter."));
		return;
	}

	const FVector2D MoveVector = Value.Get<FVector2D>();

	ControlledCharacter->Move(MoveVector);
}

void ASPlayerController::Look(const FInputActionValue& Value) {
	ASCharacter* ControlledCharacter = Cast<ASCharacter>(GetPawn());
	if (!IsValid(ControlledCharacter)) {
		UE_LOG(LogTemp, Warning, TEXT("Look failed: controlled pawn is not ASCharacter."));
		return;
	}

	FVector2D LookVector = Value.Get<FVector2D>();

	const USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
	if (IsValid(SGameInstance))
	{
		const FSGameOptionsSettings& OptionsSettings = SGameInstance->GetOptionsSettings();
		LookVector *= OptionsSettings.MouseSensitivity;

		if (OptionsSettings.bInvertMouseY)
		{
			LookVector.Y *= -1.0f;
		}
	}

	ControlledCharacter->Look(LookVector);
}

void ASPlayerController::Interact() {
	ASCharacter* ControlledCharacter = Cast<ASCharacter>(GetPawn());
	if (!IsValid(ControlledCharacter)) {
		UE_LOG(LogTemp, Warning, TEXT("Interact failed: controlled pawn is not ASCharacter."));
		return;
	}

	ControlledCharacter->TryInteract();

	UpdateInteractionPrompt();
}

void ASPlayerController::StartSprint() {
	ASCharacter* ControlledCharacter = Cast<ASCharacter>(GetPawn());
	if (!IsValid(ControlledCharacter)) {
		UE_LOG(LogTemp, Warning, TEXT("StartSprint failed: controlled pawn is not ASCharacter."));
		return;
	}

	if (!ShouldHoldToSprint())
	{
		bSprintToggled = !bSprintToggled;
		if (!bSprintToggled)
		{
			ControlledCharacter->StopSprint();
			return;
		}
	}

	ControlledCharacter->StartSprint();
}

void ASPlayerController::StopSprint() {
	if (!ShouldHoldToSprint())
	{
		return;
	}

	ASCharacter* ControlledCharacter = Cast<ASCharacter>(GetPawn());
	if (!IsValid(ControlledCharacter)) {
		UE_LOG(LogTemp, Warning, TEXT("StopSprint failed: controlled pawn is not ASCharacter."));
		return;
	}

	ControlledCharacter->StopSprint();
}

bool ASPlayerController::ShouldHoldToSprint() const
{
	const USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
	if (!IsValid(SGameInstance))
	{
		return true;
	}

	return SGameInstance->GetOptionsSettings().bHoldToSprint;
}

void ASPlayerController::TogglePause()
{
	if (IsValid(InventoryMenuWidget) && InventoryMenuWidget->IsInventoryOpen())
	{
		CloseInventory();
		return;
	}

	const bool bIsPaused = UGameplayStatics::IsGamePaused(this);

	if (bIsPaused)
	{
		ResumeGame();
		return;
	}

	PauseGame();
}

void ASPlayerController::ToggleInventory()
{
	if (IsValid(InventoryMenuWidget) && InventoryMenuWidget->IsInventoryOpen())
	{
		CloseInventory();
		return;
	}

	if (UGameplayStatics::IsGamePaused(this))
	{
		return;
	}

	OpenInventory();
}

void ASPlayerController::PauseGame()
{
    SetPausedGamePhase(true);
    SetPause(true);

    if (IsValid(InteractionPromptWidget))
    {
        InteractionPromptWidget->HidePrompt();
    }

    ShowPauseMenu();

    FInputModeGameAndUI InputMode;
    InputMode.SetHideCursorDuringCapture(false);
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    if (IsValid(PauseMenuWidget))
    {
        InputMode.SetWidgetToFocus(PauseMenuWidget->TakeWidget());
    }

    SetInputMode(InputMode);
    bShowMouseCursor = true;

    UE_LOG(LogTemp, Log, TEXT("Game paused."));
}

void ASPlayerController::ResumeGame()
{
    SetPause(false);
    SetPausedGamePhase(false);

    HidePauseMenu();

    SetInputMode(FInputModeGameOnly());
    bShowMouseCursor = false;

    UE_LOG(LogTemp, Log, TEXT("Game resumed."));
}

void ASPlayerController::CloseInventory()
{
	if (!IsValid(InventoryMenuWidget))
	{
		return;
	}

	InventoryMenuWidget->CloseInventory();

	SetPause(false);
	SetPausedGamePhase(false);

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;

	UE_LOG(LogTemp, Log, TEXT("Inventory closed."));
}

void ASPlayerController::RequestSaveGame()
{
    USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
    if (!IsValid(SGameInstance))
    {
        UE_LOG(LogTemp, Error, TEXT("RequestSaveGame failed: USGameInstance is invalid."));
        return;
    }

    const bool bSaved = SGameInstance->SaveActiveSlot();

    UE_LOG(
        LogTemp,
        Log,
        TEXT("RequestSaveGame result: %s"),
        bSaved ? TEXT("success") : TEXT("failed")
    );
}

void ASPlayerController::RequestLoadGame()
{
    USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
    if (!IsValid(SGameInstance))
    {
        UE_LOG(LogTemp, Error, TEXT("RequestLoadGame failed: USGameInstance is invalid."));
        return;
    }

    const bool bLoaded = SGameInstance->LoadActiveSlotInCurrentWorld();

    UE_LOG(
        LogTemp,
        Log,
        TEXT("RequestLoadGame result: %s"),
        bLoaded ? TEXT("success") : TEXT("failed")
    );

    if (bLoaded)
    {
        ResumeGame();
    }
}

void ASPlayerController::RequestReturnToMainMenu()
{
    USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
    if (!IsValid(SGameInstance))
    {
        UE_LOG(LogTemp, Error, TEXT("RequestReturnToMainMenu failed: USGameInstance is invalid."));
        return;
    }

    SetPause(false);
    SetPausedGamePhase(false);

    SGameInstance->ReturnToMainMenu();
}

void ASPlayerController::CreatePauseMenuWidget()
{
    if (!IsValid(PauseMenuWidgetClass))
    {
        PauseMenuWidgetClass = LoadClass<USPauseMenuWidget>(nullptr, TEXT("/Game/UI/BP_PauseMenu.BP_PauseMenu_C"));
        if (!IsValid(PauseMenuWidgetClass))
        {
            UE_LOG(LogTemp, Warning, TEXT("PauseMenuWidgetClass is not assigned."));
            return;
        }
    }

    PauseMenuWidget = CreateWidget<USPauseMenuWidget>(
        this,
        PauseMenuWidgetClass
    );

    if (!IsValid(PauseMenuWidget))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create PauseMenuWidget."));
        return;
    }

    PauseMenuWidget->AddToViewport(50);
    HidePauseMenu();
}

void ASPlayerController::CreateInventoryMenuWidget()
{
	if (!IsValid(InventoryMenuWidgetClass))
	{
		InventoryMenuWidgetClass = LoadClass<USInventoryMenuWidget>(nullptr, TEXT("/Game/UI/BP_InventoryMenu.BP_InventoryMenu_C"));
		if (!IsValid(InventoryMenuWidgetClass))
		{
			UE_LOG(LogTemp, Warning, TEXT("InventoryMenuWidgetClass is not assigned."));
			return;
		}
	}

	InventoryMenuWidget = CreateWidget<USInventoryMenuWidget>(this, InventoryMenuWidgetClass);
	if (!IsValid(InventoryMenuWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create InventoryMenuWidget."));
		return;
	}

	InventoryMenuWidget->AddToViewport(75);
	InventoryMenuWidget->CloseInventory();
}

void ASPlayerController::OpenInventory()
{
	if (!IsValid(InventoryMenuWidget))
	{
		CreateInventoryMenuWidget();
	}

	if (!IsValid(InventoryMenuWidget))
	{
		return;
	}

	ASCharacter* ControlledCharacter = Cast<ASCharacter>(GetPawn());
	if (!IsValid(ControlledCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("OpenInventory failed: controlled pawn is not ASCharacter."));
		return;
	}

	USInventoryComponent* InventoryComponent = ControlledCharacter->GetInventoryComponent();
	if (!IsValid(InventoryComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("OpenInventory failed: InventoryComponent is invalid."));
		return;
	}

	SetPausedGamePhase(true);
	SetPause(true);

	if (IsValid(InteractionPromptWidget))
	{
		InteractionPromptWidget->HidePrompt();
	}

	HidePauseMenu();
	InventoryMenuWidget->OpenForInventory(InventoryComponent);

	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(InventoryMenuWidget->TakeWidget());
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
	bShowMouseCursor = true;

	UE_LOG(LogTemp, Log, TEXT("Inventory opened."));
}

void ASPlayerController::ShowPauseMenu()
{
    if (!IsValid(PauseMenuWidget))
    {
        return;
    }

    PauseMenuWidget->SetVisibility(ESlateVisibility::Visible);
}

void ASPlayerController::HidePauseMenu()
{
    if (!IsValid(PauseMenuWidget))
    {
        return;
    }

    PauseMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void ASPlayerController::SetPausedGamePhase(bool bPaused)
{
    UWorld* World = GetWorld();
    if (!IsValid(World))
    {
        return;
    }

    ASGameState* SGameState = World->GetGameState<ASGameState>();
    if (!IsValid(SGameState))
    {
        return;
    }

    if (SGameState->GetGamePhase() == ESGamePhase::GameOver)
    {
        return;
    }

    if (bPaused)
    {
        SGameState->SetGamePhase(ESGamePhase::Paused);
        return;
    }

    if (SGameState->GetGamePhase() == ESGamePhase::Paused)
    {
        SGameState->SetGamePhase(ESGamePhase::Playing);
    }
}

void ASPlayerController::CreateInteractionPromptWidget() {
	if (!IsValid(InteractionPromptWidgetClass)) {
		UE_LOG(LogTemp, Warning, TEXT("InteractionPromptWidgetClass is not assigned."));
		return;
	}

	InteractionPromptWidget = CreateWidget<USInteractionPromptWidget>(
		this,
		InteractionPromptWidgetClass
	);

	if (!IsValid(InteractionPromptWidget)) {
		UE_LOG(LogTemp, Error, TEXT("Failed to create InteractionPromptWidget."));
		return;
	}

	InteractionPromptWidget->AddToViewport();
	InteractionPromptWidget->HidePrompt();
}
