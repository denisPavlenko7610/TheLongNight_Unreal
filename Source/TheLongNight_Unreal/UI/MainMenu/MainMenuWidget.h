// #pragma once
//
// #include "CoreMinimal.h"
// #include "Blueprint/UserWidget.h"
// #include "MainMenuWidget.generated.h"
//
// class UMenuButtonWidget;
// class UButton;
//
// UCLASS()
// class THELONGNIGHT_UNREAL_API UMainMenuWidget : public UUserWidget
// {
// 	GENERATED_BODY()
//
// protected:
// 	virtual void NativeOnInitialized() override;
//
// 	UPROPERTY(meta = (BindWidget))
// 	UButton* NewButton;
//
// 	UPROPERTY(meta = (BindWidget))
// 	UButton* LoadButton;
//
// 	UPROPERTY(meta = (BindWidget))
// 	UButton* OptionsButton;
//
// 	UPROPERTY(meta = (BindWidget))
// 	UButton* ExitButton;
//
// 	UFUNCTION()
// 	void NewGame();
//
// 	UFUNCTION()
// 	void LoadGame();
//
// 	UFUNCTION()
// 	void Options();
//
// 	UFUNCTION()
// 	void Exit();
// };
