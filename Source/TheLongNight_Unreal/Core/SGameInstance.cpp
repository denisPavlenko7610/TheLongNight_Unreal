#include "Core/SGameInstance.h"

#include "Engine/World.h"
#include "Items/SItemRegistryData.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/App.h"
#include "Save/SGameSaveSubsystem.h"
#include "Save/SOptionsSaveObject.h"

namespace
{
	constexpr int32 OptionsSaveUserIndex = 0;
}

void USGameInstance::Init()
{
	Super::Init();

	LoadOptionsSettings();
	ApplyCustomOptionsSettings();

	UE_LOG(LogTemp, Log, TEXT("USGameInstance initialized."));
}

void USGameInstance::Shutdown()
{
	UE_LOG(LogTemp, Log, TEXT("USGameInstance shutdown."));

	Super::Shutdown();
}

void USGameInstance::StartNewGame()
{
	StartGameMode = ESTypes::NewGame;
	ActiveSaveSlotName = TEXT("Slot_01");

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("StartNewGame failed: World is invalid."));
		return;
	}

	UGameplayStatics::OpenLevel(World, OpenWorldMapName);
}

USItemData* USGameInstance::FindItemById(FName ItemId) const
{
	if (!IsValid(ItemRegistryData))
	{
		UE_LOG(LogTemp, Error, TEXT("FindItemById failed: ItemRegistryData is not assigned."));
		return nullptr;
	}

	return ItemRegistryData->FindItemById(ItemId);
}

void USGameInstance::LoadGame()
{
	StartGameMode = ESTypes::LoadGame;

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("LoadGame failed: World is invalid."));
		return;
	}

	UGameplayStatics::OpenLevel(World, OpenWorldMapName);
}

void USGameInstance::ReturnToMainMenu()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("ReturnToMainMenu failed: World is invalid."));
		return;
	}

	UGameplayStatics::OpenLevel(World, MainMenuMapName);
}

void USGameInstance::SetActiveSaveSlotName(const FString& NewSlotName)
{
	if (NewSlotName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("SetActiveSaveSlotName ignored: slot name is empty."));
		return;
	}

	ActiveSaveSlotName = NewSlotName;
}

FString USGameInstance::GetActiveSaveSlotName() const
{
	return ActiveSaveSlotName;
}

ESTypes USGameInstance::GetStartGameMode() const
{
	return StartGameMode;
}

bool USGameInstance::SaveActiveSlot()
{
	if (ActiveSaveSlotName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveActiveSlot failed: ActiveSaveSlotName is empty."));
		return false;
	}

	USGameSaveSubsystem* SaveSubsystem = GetSubsystem<USGameSaveSubsystem>();
	if (!IsValid(SaveSubsystem))
	{
		UE_LOG(LogTemp, Error, TEXT("SaveActiveSlot failed: SaveSubsystem is invalid."));
		return false;
	}

	return SaveSubsystem->SaveCurrentGame(ActiveSaveSlotName);
}

bool USGameInstance::LoadActiveSlotInCurrentWorld()
{
	if (ActiveSaveSlotName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadActiveSlotInCurrentWorld failed: ActiveSaveSlotName is empty."));
		return false;
	}

	USGameSaveSubsystem* SaveSubsystem = GetSubsystem<USGameSaveSubsystem>();
	if (!IsValid(SaveSubsystem))
	{
		UE_LOG(LogTemp, Error, TEXT("LoadActiveSlotInCurrentWorld failed: SaveSubsystem is invalid."));
		return false;
	}

	return SaveSubsystem->LoadCurrentGame(ActiveSaveSlotName);
}

const FSGameOptionsSettings& USGameInstance::GetOptionsSettings() const
{
	return OptionsSettings;
}

FSGameOptionsSettings USGameInstance::GetOptionsSettingsCopy() const
{
	return OptionsSettings;
}

void USGameInstance::SetOptionsSettings(const FSGameOptionsSettings& NewSettings, bool bSaveImmediately)
{
	OptionsSettings = NewSettings;
	OptionsSettings.MasterVolume = FMath::Clamp(OptionsSettings.MasterVolume, 0.0f, 1.0f);
	OptionsSettings.MusicVolume = FMath::Clamp(OptionsSettings.MusicVolume, 0.0f, 1.0f);
	OptionsSettings.SfxVolume = FMath::Clamp(OptionsSettings.SfxVolume, 0.0f, 1.0f);
	OptionsSettings.MouseSensitivity = FMath::Clamp(OptionsSettings.MouseSensitivity, 0.1f, 3.0f);

	ApplyCustomOptionsSettings();

	if (bSaveImmediately)
	{
		SaveOptionsSettings();
	}
}

void USGameInstance::LoadOptionsSettings()
{
	USaveGame* LoadedObject = UGameplayStatics::LoadGameFromSlot(GetOptionsSaveSlotName(), OptionsSaveUserIndex);
	USOptionsSaveObject* LoadedOptions = Cast<USOptionsSaveObject>(LoadedObject);
	if (!IsValid(LoadedOptions))
	{
		OptionsSettings = FSGameOptionsSettings();
		return;
	}

	SetOptionsSettings(LoadedOptions->Settings, false);
}

bool USGameInstance::SaveOptionsSettings() const
{
	USOptionsSaveObject* SaveObject = Cast<USOptionsSaveObject>(
		UGameplayStatics::CreateSaveGameObject(USOptionsSaveObject::StaticClass())
	);

	if (!IsValid(SaveObject))
	{
		UE_LOG(LogTemp, Error, TEXT("SaveOptionsSettings failed: SaveObject is invalid."));
		return false;
	}

	SaveObject->Settings = OptionsSettings;

	return UGameplayStatics::SaveGameToSlot(SaveObject, GetOptionsSaveSlotName(), OptionsSaveUserIndex);
}

void USGameInstance::ApplyCustomOptionsSettings() const
{
	FApp::SetVolumeMultiplier(OptionsSettings.MasterVolume);
}

FString USGameInstance::GetOptionsSaveSlotName()
{
	return TEXT("Options");
}
