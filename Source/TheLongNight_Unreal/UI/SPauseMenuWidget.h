#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPauseMenuWidget.generated.h"

class UButton;
class UOptionsMenuWidget;

UCLASS()
class THELONGNIGHT_UNREAL_API USPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void HandleResumeClicked();

	UFUNCTION()
	void HandleSaveClicked();

	UFUNCTION()
	void HandleLoadClicked();

	UFUNCTION()
	void HandleMainMenuClicked();

	UFUNCTION()
	void HandleOptionsClicked();

	UFUNCTION()
	void ResumeGame();

	UFUNCTION()
	void SaveGame();

	UFUNCTION()
	void LoadGame();

	UFUNCTION()
	void Options();

	UFUNCTION()
	void HandleExit();

	void CreateOptionsMenuWidget();

private:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> ResumeButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> SaveButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> LoadButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> MainMenuButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> ExitButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> OptionsButton;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|UI")
	TSubclassOf<UOptionsMenuWidget> OptionsMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UOptionsMenuWidget> OptionsMenuWidget;
};
