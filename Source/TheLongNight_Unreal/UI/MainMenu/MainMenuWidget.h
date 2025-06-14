#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UMenuButtonWidget;
class UButton;

UCLASS()
class THELONGNIGHT_UNREAL_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	UButton* NewGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* LoadGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* OptionsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

	UFUNCTION()
	void HandleNewGame();

	UFUNCTION()
	void HandleLoadGame();

	UFUNCTION()
	void HandleOptions();

	UFUNCTION()
	void HandleExit();
};
