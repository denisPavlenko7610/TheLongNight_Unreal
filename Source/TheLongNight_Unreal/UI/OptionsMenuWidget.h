#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GenericPlatform/GenericWindow.h"
#include "OptionsMenuWidget.generated.h"

class UButton;
class UCheckBox;
class UComboBoxString;
class UTextBlock;
class USlider;
class UWidget;
class UWidgetSwitcher;

UCLASS()
class THELONGNIGHT_UNREAL_API UOptionsMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void OpenFrom(UWidget* InReturnWidget);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void BindEvents();
	void UnbindEvents();

	void PopulateStaticOptions() const;
	void PopulateDisplayOptions();
	void RefreshFromSavedSettings();
	void RefreshValueLabels();
	void SelectTab(int32 TabIndex) const;

	FIntPoint GetSelectedResolution() const;
	static FString FormatResolution(const FIntPoint& Resolution);
	static FString WindowModeToOption(EWindowMode::Type WindowMode);
	static EWindowMode::Type OptionToWindowMode(const FString& Option);
	static FString QualityLevelToOption(int32 QualityLevel);
	static int32 OptionToQualityLevel(const FString& Option);
	static FString FrameLimitToOption(float FrameLimit);
	static float OptionToFrameLimit(const FString& Option);

	void ApplyDisplaySettings() const;
	void ApplyCustomSettings() const;

	UFUNCTION()
	void HandleDisplayTabClicked();

	UFUNCTION()
	void HandleAudioTabClicked();

	UFUNCTION()
	void HandleGameplayTabClicked();

	UFUNCTION()
	void HandleApplyClicked();

	UFUNCTION()
	void HandleDefaultsClicked();

	UFUNCTION()
	void HandleBackClicked();

	UFUNCTION()
	void HandleMasterVolumeChanged(float Value);

	UFUNCTION()
	void HandleMusicVolumeChanged(float Value);

	UFUNCTION()
	void HandleSfxVolumeChanged(float Value);

	UFUNCTION()
	void HandleMouseSensitivityChanged(float Value);

private:
	TWeakObjectPtr<UWidget> ReturnWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> PageSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> DisplayTabButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> AudioTabButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> GameplayTabButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ApplyButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> DefaultsButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BackButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UComboBoxString> ResolutionComboBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UComboBoxString> WindowModeComboBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UComboBoxString> QualityComboBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UComboBoxString> FrameRateComboBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> VSyncCheckBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> MasterVolumeSlider;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MasterVolumeValueText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> MusicVolumeSlider;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MusicVolumeValueText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> SfxVolumeSlider;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SfxVolumeValueText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> MouseSensitivitySlider;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MouseSensitivityValueText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> InvertMouseYCheckBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> HoldToSprintCheckBox;
};
