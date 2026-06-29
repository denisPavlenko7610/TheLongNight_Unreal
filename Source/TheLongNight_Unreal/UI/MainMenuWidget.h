#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UOptionsMenuWidget;

UCLASS()
class THELONGNIGHT_UNREAL_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void NewGame();

	UFUNCTION()
	void LoadGame();

	UFUNCTION()
	void Options();

	UFUNCTION()
	void Exit();

	void CreateOptionsMenuWidget();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> NewButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> LoadButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> OptionsButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|UI")
	TSubclassOf<UOptionsMenuWidget> OptionsMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UOptionsMenuWidget> OptionsMenuWidget;
};
