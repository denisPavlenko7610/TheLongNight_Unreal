#include "Core/SGameInstance.h"

#include "Engine/World.h"
#include "Items/SItemRegistryData.h"
#include "Kismet/GameplayStatics.h"

void USGameInstance::Init()
{
	Super::Init();

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
