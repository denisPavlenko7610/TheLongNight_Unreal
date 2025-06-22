#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

class UMenuButtonWidget;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResumeClicked);

UCLASS()
class THELONGNIGHT_UNREAL_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, Category = "Pause")
	FOnResumeClicked OnResumeClickedDelegate;
	
protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton;

	UPROPERTY(meta = (BindWidget))
	UButton* LoadButton;

	UPROPERTY(meta = (BindWidget))
	UButton* OptionsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

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
};
