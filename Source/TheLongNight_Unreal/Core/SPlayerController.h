#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

class USInteractionPromptWidget;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class THELONGNIGHT_UNREAL_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASPlayerController();

	void UpdateInteractionPrompt();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact();
	void StartSprint();
	void StopSprint();
	void TogglePause();

	void CreateInteractionPromptWidget();

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
	int32 GameplayMappingPriority = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|UI")
	TSubclassOf<USInteractionPromptWidget> InteractionPromptWidgetClass;

	UPROPERTY()
	TObjectPtr<USInteractionPromptWidget> InteractionPromptWidget;

	FTimerHandle InteractionPromptTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|UI")
	float InteractionPromptUpdateInterval = 0.1f;
};
