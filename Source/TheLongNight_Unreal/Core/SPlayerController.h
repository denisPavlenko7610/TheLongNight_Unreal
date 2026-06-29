#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

class USInteractionPromptWidget;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class USPauseMenuWidget;
class USInventoryMenuWidget;

UCLASS()
class THELONGNIGHT_UNREAL_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASPlayerController();

	UFUNCTION(BlueprintCallable, Category = "Survival|Pause")
	void ResumeGame();

	UFUNCTION(BlueprintCallable, Category = "Survival|Save")
	void RequestSaveGame();

	UFUNCTION(BlueprintCallable, Category = "Survival|Save")
	void RequestLoadGame();

	UFUNCTION(BlueprintCallable, Category = "Survival|Flow")
	void RequestReturnToMainMenu();

	UFUNCTION(BlueprintCallable, Category = "Survival|Inventory")
	void CloseInventory();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact();
	void StartSprint();
	void StopSprint();
	bool ShouldHoldToSprint() const;
	void TogglePause();
	void ToggleInventory();
	void UpdateInteractionPrompt();

	void CreateInteractionPromptWidget();

	void PauseGame();
	void CreatePauseMenuWidget();
	void ShowPauseMenu();
	void HidePauseMenu();
	void CreateInventoryMenuWidget();
	void OpenInventory();
	void SetPausedGamePhase(bool bPaused);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Survival|Input")
	TObjectPtr<UInputMappingContext> GameplayMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Input")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Input")
	TObjectPtr<UInputAction> PauseAction;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Input")
	TObjectPtr<UInputAction> ToggleInventoryAction;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Input")
	int32 GameplayMappingPriority = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|UI")
	TSubclassOf<USInteractionPromptWidget> InteractionPromptWidgetClass;

	UPROPERTY()
	TObjectPtr<USInteractionPromptWidget> InteractionPromptWidget;

	FTimerHandle InteractionPromptTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|UI")
	float InteractionPromptUpdateInterval = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|UI")
	TSubclassOf<USPauseMenuWidget> PauseMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<USPauseMenuWidget> PauseMenuWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|UI")
	TSubclassOf<USInventoryMenuWidget> InventoryMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<USInventoryMenuWidget> InventoryMenuWidget;

	bool bSprintToggled = false;
};
