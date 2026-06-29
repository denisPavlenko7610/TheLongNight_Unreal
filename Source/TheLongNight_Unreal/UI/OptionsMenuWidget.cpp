#include "UI/OptionsMenuWidget.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Core/SGameInstance.h"
#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

namespace
{
	FText FormatPercentText(float Value)
	{
		return FText::FromString(FString::Printf(TEXT("%d%%"), FMath::RoundToInt(Value * 100.0f)));
	}

	FText FormatMultiplierText(float Value)
	{
		return FText::FromString(FString::Printf(TEXT("%.2fx"), Value));
	}

	void ResetComboOptions(UComboBoxString* ComboBox, const TArray<FString>& Options)
	{
		if (!IsValid(ComboBox))
		{
			return;
		}

		ComboBox->ClearOptions();
		for (const FString& Option : Options)
		{
			ComboBox->AddOption(Option);
		}
	}
}

void UOptionsMenuWidget::OpenFrom(UWidget* InReturnWidget)
{
	ReturnWidget = InReturnWidget;

	if (!IsInViewport())
	{
		AddToViewport(100);
	}

	if (ReturnWidget.IsValid())
	{
		ReturnWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	RefreshFromSavedSettings();
	SetVisibility(ESlateVisibility::Visible);

	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController))
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}
}

void UOptionsMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PopulateStaticOptions();
	BindEvents();
	RefreshFromSavedSettings();
	SelectTab(0);
	SetVisibility(ESlateVisibility::Collapsed);
}

void UOptionsMenuWidget::NativeDestruct()
{
	UnbindEvents();

	Super::NativeDestruct();
}

void UOptionsMenuWidget::BindEvents()
{
	UnbindEvents();

	if (IsValid(DisplayTabButton))
	{
		DisplayTabButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::HandleDisplayTabClicked);
	}

	if (IsValid(AudioTabButton))
	{
		AudioTabButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::HandleAudioTabClicked);
	}

	if (IsValid(GameplayTabButton))
	{
		GameplayTabButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::HandleGameplayTabClicked);
	}

	if (IsValid(ApplyButton))
	{
		ApplyButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::HandleApplyClicked);
	}

	if (IsValid(DefaultsButton))
	{
		DefaultsButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::HandleDefaultsClicked);
	}

	if (IsValid(BackButton))
	{
		BackButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::HandleBackClicked);
	}

	if (IsValid(MasterVolumeSlider))
	{
		MasterVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionsMenuWidget::HandleMasterVolumeChanged);
	}

	if (IsValid(MusicVolumeSlider))
	{
		MusicVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionsMenuWidget::HandleMusicVolumeChanged);
	}

	if (IsValid(SfxVolumeSlider))
	{
		SfxVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionsMenuWidget::HandleSfxVolumeChanged);
	}

	if (IsValid(MouseSensitivitySlider))
	{
		MouseSensitivitySlider->OnValueChanged.AddDynamic(this, &UOptionsMenuWidget::HandleMouseSensitivityChanged);
	}
}

void UOptionsMenuWidget::UnbindEvents()
{
	if (IsValid(DisplayTabButton))
	{
		DisplayTabButton->OnClicked.RemoveDynamic(this, &UOptionsMenuWidget::HandleDisplayTabClicked);
	}

	if (IsValid(AudioTabButton))
	{
		AudioTabButton->OnClicked.RemoveDynamic(this, &UOptionsMenuWidget::HandleAudioTabClicked);
	}

	if (IsValid(GameplayTabButton))
	{
		GameplayTabButton->OnClicked.RemoveDynamic(this, &UOptionsMenuWidget::HandleGameplayTabClicked);
	}

	if (IsValid(ApplyButton))
	{
		ApplyButton->OnClicked.RemoveDynamic(this, &UOptionsMenuWidget::HandleApplyClicked);
	}

	if (IsValid(DefaultsButton))
	{
		DefaultsButton->OnClicked.RemoveDynamic(this, &UOptionsMenuWidget::HandleDefaultsClicked);
	}

	if (IsValid(BackButton))
	{
		BackButton->OnClicked.RemoveDynamic(this, &UOptionsMenuWidget::HandleBackClicked);
	}

	if (IsValid(MasterVolumeSlider))
	{
		MasterVolumeSlider->OnValueChanged.RemoveDynamic(this, &UOptionsMenuWidget::HandleMasterVolumeChanged);
	}

	if (IsValid(MusicVolumeSlider))
	{
		MusicVolumeSlider->OnValueChanged.RemoveDynamic(this, &UOptionsMenuWidget::HandleMusicVolumeChanged);
	}

	if (IsValid(SfxVolumeSlider))
	{
		SfxVolumeSlider->OnValueChanged.RemoveDynamic(this, &UOptionsMenuWidget::HandleSfxVolumeChanged);
	}

	if (IsValid(MouseSensitivitySlider))
	{
		MouseSensitivitySlider->OnValueChanged.RemoveDynamic(this, &UOptionsMenuWidget::HandleMouseSensitivityChanged);
	}
}

void UOptionsMenuWidget::PopulateStaticOptions() const
{
	ResetComboOptions(WindowModeComboBox, {
		TEXT("Fullscreen"),
		TEXT("Borderless"),
		TEXT("Windowed")
	});

	ResetComboOptions(FrameRateComboBox, {
		TEXT("Unlimited"),
		TEXT("30 FPS"),
		TEXT("60 FPS"),
		TEXT("120 FPS"),
		TEXT("144 FPS")
	});

	ResetComboOptions(QualityComboBox, {
		TEXT("Low"),
		TEXT("Medium"),
		TEXT("High"),
		TEXT("Epic"),
		TEXT("Cinematic"),
		TEXT("Custom")
	});
}

void UOptionsMenuWidget::PopulateDisplayOptions()
{
	if (!IsValid(ResolutionComboBox))
	{
		return;
	}

	ResolutionComboBox->ClearOptions();

	UGameUserSettings* UserSettings = GEngine ? GEngine->GetGameUserSettings() : nullptr;
	FIntPoint CurrentResolution(1920, 1080);
	if (IsValid(UserSettings))
	{
		CurrentResolution = UserSettings->GetScreenResolution();
	}

	if (CurrentResolution.X <= 0 || CurrentResolution.Y <= 0)
	{
		CurrentResolution = FIntPoint(1920, 1080);
	}

	TArray<FIntPoint> Resolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);

	if (Resolutions.IsEmpty())
	{
		Resolutions = {
			FIntPoint(1280, 720),
			FIntPoint(1600, 900),
			FIntPoint(1920, 1080),
			FIntPoint(2560, 1440),
			FIntPoint(3840, 2160)
		};
	}

	if (!Resolutions.Contains(CurrentResolution))
	{
		Resolutions.Add(CurrentResolution);
	}

	Resolutions.Sort([](const FIntPoint& Left, const FIntPoint& Right)
	{
		return Left.X == Right.X ? Left.Y < Right.Y : Left.X < Right.X;
	});

	FIntPoint LastResolution(INDEX_NONE, INDEX_NONE);
	for (const FIntPoint& Resolution : Resolutions)
	{
		if (Resolution == LastResolution || Resolution.X <= 0 || Resolution.Y <= 0)
		{
			continue;
		}

		ResolutionComboBox->AddOption(FormatResolution(Resolution));
		LastResolution = Resolution;
	}

	ResolutionComboBox->SetSelectedOption(FormatResolution(CurrentResolution));
}

void UOptionsMenuWidget::RefreshFromSavedSettings()
{
	PopulateDisplayOptions();

	UGameUserSettings* UserSettings = GEngine ? GEngine->GetGameUserSettings() : nullptr;
	if (IsValid(UserSettings))
	{
		if (IsValid(WindowModeComboBox))
		{
			WindowModeComboBox->SetSelectedOption(WindowModeToOption(UserSettings->GetFullscreenMode()));
		}

		if (IsValid(VSyncCheckBox))
		{
			VSyncCheckBox->SetIsChecked(UserSettings->IsVSyncEnabled());
		}

		if (IsValid(FrameRateComboBox))
		{
			FrameRateComboBox->SetSelectedOption(FrameLimitToOption(UserSettings->GetFrameRateLimit()));
		}

		if (IsValid(QualityComboBox))
		{
			QualityComboBox->SetSelectedOption(QualityLevelToOption(UserSettings->GetOverallScalabilityLevel()));
		}
	}

	FSGameOptionsSettings OptionsSettings;
	const USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
	if (IsValid(SGameInstance))
	{
		OptionsSettings = SGameInstance->GetOptionsSettings();
	}

	if (IsValid(MasterVolumeSlider))
	{
		MasterVolumeSlider->SetValue(OptionsSettings.MasterVolume);
	}

	if (IsValid(MusicVolumeSlider))
	{
		MusicVolumeSlider->SetValue(OptionsSettings.MusicVolume);
	}

	if (IsValid(SfxVolumeSlider))
	{
		SfxVolumeSlider->SetValue(OptionsSettings.SfxVolume);
	}

	if (IsValid(MouseSensitivitySlider))
	{
		MouseSensitivitySlider->SetValue(OptionsSettings.MouseSensitivity);
	}

	if (IsValid(InvertMouseYCheckBox))
	{
		InvertMouseYCheckBox->SetIsChecked(OptionsSettings.bInvertMouseY);
	}

	if (IsValid(HoldToSprintCheckBox))
	{
		HoldToSprintCheckBox->SetIsChecked(OptionsSettings.bHoldToSprint);
	}

	RefreshValueLabels();
}

void UOptionsMenuWidget::RefreshValueLabels()
{
	if (IsValid(MasterVolumeSlider) && IsValid(MasterVolumeValueText))
	{
		MasterVolumeValueText->SetText(FormatPercentText(MasterVolumeSlider->GetValue()));
	}

	if (IsValid(MusicVolumeSlider) && IsValid(MusicVolumeValueText))
	{
		MusicVolumeValueText->SetText(FormatPercentText(MusicVolumeSlider->GetValue()));
	}

	if (IsValid(SfxVolumeSlider) && IsValid(SfxVolumeValueText))
	{
		SfxVolumeValueText->SetText(FormatPercentText(SfxVolumeSlider->GetValue()));
	}

	if (IsValid(MouseSensitivitySlider) && IsValid(MouseSensitivityValueText))
	{
		MouseSensitivityValueText->SetText(FormatMultiplierText(MouseSensitivitySlider->GetValue()));
	}
}

void UOptionsMenuWidget::SelectTab(int32 TabIndex) const
{
	if (IsValid(PageSwitcher))
	{
		PageSwitcher->SetActiveWidgetIndex(TabIndex);
	}
}

FIntPoint UOptionsMenuWidget::GetSelectedResolution() const
{
	if (!IsValid(ResolutionComboBox))
	{
		return FIntPoint(1920, 1080);
	}

	FString WidthString;
	FString HeightString;
	if (!ResolutionComboBox->GetSelectedOption().Split(TEXT(" x "), &WidthString, &HeightString))
	{
		return FIntPoint(1920, 1080);
	}

	return FIntPoint(FCString::Atoi(*WidthString), FCString::Atoi(*HeightString));
}

FString UOptionsMenuWidget::FormatResolution(const FIntPoint& Resolution)
{
	return FString::Printf(TEXT("%d x %d"), Resolution.X, Resolution.Y);
}

FString UOptionsMenuWidget::WindowModeToOption(EWindowMode::Type WindowMode)
{
	switch (WindowMode)
	{
	case EWindowMode::Fullscreen:
		return TEXT("Fullscreen");
	case EWindowMode::Windowed:
		return TEXT("Windowed");
	case EWindowMode::WindowedFullscreen:
	default:
		return TEXT("Borderless");
	}
}

EWindowMode::Type UOptionsMenuWidget::OptionToWindowMode(const FString& Option)
{
	if (Option == TEXT("Fullscreen"))
	{
		return EWindowMode::Fullscreen;
	}

	if (Option == TEXT("Windowed"))
	{
		return EWindowMode::Windowed;
	}

	return EWindowMode::WindowedFullscreen;
}

FString UOptionsMenuWidget::QualityLevelToOption(int32 QualityLevel)
{
	switch (QualityLevel)
	{
	case 0:
		return TEXT("Low");
	case 1:
		return TEXT("Medium");
	case 2:
		return TEXT("High");
	case 3:
		return TEXT("Epic");
	case 4:
		return TEXT("Cinematic");
	default:
		return TEXT("Custom");
	}
}

int32 UOptionsMenuWidget::OptionToQualityLevel(const FString& Option)
{
	if (Option == TEXT("Low"))
	{
		return 0;
	}

	if (Option == TEXT("Medium"))
	{
		return 1;
	}

	if (Option == TEXT("High"))
	{
		return 2;
	}

	if (Option == TEXT("Epic"))
	{
		return 3;
	}

	if (Option == TEXT("Cinematic"))
	{
		return 4;
	}

	return INDEX_NONE;
}

FString UOptionsMenuWidget::FrameLimitToOption(float FrameLimit)
{
	if (FrameLimit <= 0.0f)
	{
		return TEXT("Unlimited");
	}

	const int32 RoundedLimit = FMath::RoundToInt(FrameLimit);
	if (RoundedLimit <= 45)
	{
		return TEXT("30 FPS");
	}

	if (RoundedLimit <= 90)
	{
		return TEXT("60 FPS");
	}

	if (RoundedLimit <= 132)
	{
		return TEXT("120 FPS");
	}

	return TEXT("144 FPS");
}

float UOptionsMenuWidget::OptionToFrameLimit(const FString& Option)
{
	if (Option == TEXT("30 FPS"))
	{
		return 30.0f;
	}

	if (Option == TEXT("60 FPS"))
	{
		return 60.0f;
	}

	if (Option == TEXT("120 FPS"))
	{
		return 120.0f;
	}

	if (Option == TEXT("144 FPS"))
	{
		return 144.0f;
	}

	return 0.0f;
}

void UOptionsMenuWidget::ApplyDisplaySettings() const
{
	UGameUserSettings* UserSettings = GEngine ? GEngine->GetGameUserSettings() : nullptr;
	if (!IsValid(UserSettings))
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyDisplaySettings failed: GameUserSettings is invalid."));
		return;
	}

	const FIntPoint SelectedResolution = GetSelectedResolution();
	if (SelectedResolution.X > 0 && SelectedResolution.Y > 0)
	{
		UserSettings->SetScreenResolution(SelectedResolution);
	}

	if (IsValid(WindowModeComboBox))
	{
		UserSettings->SetFullscreenMode(OptionToWindowMode(WindowModeComboBox->GetSelectedOption()));
	}

	if (IsValid(VSyncCheckBox))
	{
		UserSettings->SetVSyncEnabled(VSyncCheckBox->IsChecked());
	}

	if (IsValid(FrameRateComboBox))
	{
		UserSettings->SetFrameRateLimit(OptionToFrameLimit(FrameRateComboBox->GetSelectedOption()));
	}

	if (IsValid(QualityComboBox))
	{
		const int32 QualityLevel = OptionToQualityLevel(QualityComboBox->GetSelectedOption());
		if (QualityLevel != INDEX_NONE)
		{
			UserSettings->SetOverallScalabilityLevel(QualityLevel);
		}
	}

	UserSettings->ApplySettings(false);
	UserSettings->SaveSettings();
}

void UOptionsMenuWidget::ApplyCustomSettings() const
{
	USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
	if (!IsValid(SGameInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyCustomSettings failed: USGameInstance is invalid."));
		return;
	}

	FSGameOptionsSettings OptionsSettings;

	if (IsValid(MasterVolumeSlider))
	{
		OptionsSettings.MasterVolume = MasterVolumeSlider->GetValue();
	}

	if (IsValid(MusicVolumeSlider))
	{
		OptionsSettings.MusicVolume = MusicVolumeSlider->GetValue();
	}

	if (IsValid(SfxVolumeSlider))
	{
		OptionsSettings.SfxVolume = SfxVolumeSlider->GetValue();
	}

	if (IsValid(MouseSensitivitySlider))
	{
		OptionsSettings.MouseSensitivity = MouseSensitivitySlider->GetValue();
	}

	if (IsValid(InvertMouseYCheckBox))
	{
		OptionsSettings.bInvertMouseY = InvertMouseYCheckBox->IsChecked();
	}

	if (IsValid(HoldToSprintCheckBox))
	{
		OptionsSettings.bHoldToSprint = HoldToSprintCheckBox->IsChecked();
	}

	SGameInstance->SetOptionsSettings(OptionsSettings, true);
}

void UOptionsMenuWidget::HandleDisplayTabClicked()
{
	SelectTab(0);
}

void UOptionsMenuWidget::HandleAudioTabClicked()
{
	SelectTab(1);
}

void UOptionsMenuWidget::HandleGameplayTabClicked()
{
	SelectTab(2);
}

void UOptionsMenuWidget::HandleApplyClicked()
{
	ApplyDisplaySettings();
	ApplyCustomSettings();
	RefreshFromSavedSettings();
}

void UOptionsMenuWidget::HandleDefaultsClicked()
{
	UGameUserSettings* UserSettings = GEngine ? GEngine->GetGameUserSettings() : nullptr;
	if (IsValid(UserSettings))
	{
		if (IsValid(WindowModeComboBox))
		{
			WindowModeComboBox->SetSelectedOption(TEXT("Borderless"));
		}

		if (IsValid(FrameRateComboBox))
		{
			FrameRateComboBox->SetSelectedOption(TEXT("Unlimited"));
		}

		if (IsValid(QualityComboBox))
		{
			QualityComboBox->SetSelectedOption(TEXT("High"));
		}

		if (IsValid(VSyncCheckBox))
		{
			VSyncCheckBox->SetIsChecked(false);
		}
	}

	const FSGameOptionsSettings DefaultOptions;
	if (IsValid(MasterVolumeSlider))
	{
		MasterVolumeSlider->SetValue(DefaultOptions.MasterVolume);
	}

	if (IsValid(MusicVolumeSlider))
	{
		MusicVolumeSlider->SetValue(DefaultOptions.MusicVolume);
	}

	if (IsValid(SfxVolumeSlider))
	{
		SfxVolumeSlider->SetValue(DefaultOptions.SfxVolume);
	}

	if (IsValid(MouseSensitivitySlider))
	{
		MouseSensitivitySlider->SetValue(DefaultOptions.MouseSensitivity);
	}

	if (IsValid(InvertMouseYCheckBox))
	{
		InvertMouseYCheckBox->SetIsChecked(DefaultOptions.bInvertMouseY);
	}

	if (IsValid(HoldToSprintCheckBox))
	{
		HoldToSprintCheckBox->SetIsChecked(DefaultOptions.bHoldToSprint);
	}

	RefreshValueLabels();
}

void UOptionsMenuWidget::HandleBackClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);

	UWidget* WidgetToFocus = nullptr;
	if (ReturnWidget.IsValid())
	{
		ReturnWidget->SetVisibility(ESlateVisibility::Visible);
		WidgetToFocus = ReturnWidget.Get();
	}

	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController))
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		if (IsValid(WidgetToFocus))
		{
			InputMode.SetWidgetToFocus(WidgetToFocus->TakeWidget());
		}

		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}
}

void UOptionsMenuWidget::HandleMasterVolumeChanged(float Value)
{
	RefreshValueLabels();
}

void UOptionsMenuWidget::HandleMusicVolumeChanged(float Value)
{
	RefreshValueLabels();
}

void UOptionsMenuWidget::HandleSfxVolumeChanged(float Value)
{
	RefreshValueLabels();
}

void UOptionsMenuWidget::HandleMouseSensitivityChanged(float Value)
{
	RefreshValueLabels();
}
