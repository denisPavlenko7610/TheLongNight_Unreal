#include "Save/SGameSaveSubsystem.h"

#include "Character/SCharacter.h"
#include "Core/ASGameState.h"
#include "Core/SGameInstance.h"
#include "Engine/World.h"
#include "Inventory/SInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Save/SGameSaveObject.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Survival/SCharacterSurvivalComponent.h"

namespace
{
	constexpr int32 DefaultSaveUserIndex = 0;
}

bool USGameSaveSubsystem::SaveCurrentGame(const FString& SlotName)
{
	if (SlotName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveCurrentGame failed: SlotName is empty."));
		return false;
	}

	FSGameSaveData SaveData;
	if (!BuildSaveData(SaveData))
	{
		return false;
	}

	USGameSaveObject* SaveObject = CreateSaveObject(SlotName, SaveData);
	if (!IsValid(SaveObject))
	{
		return false;
	}

	const bool bSaved = UGameplayStatics::SaveGameToSlot(
		SaveObject,
		SlotName,
		DefaultSaveUserIndex
	);

	UE_LOG(
		LogTemp,
		Log,
		TEXT("SaveCurrentGame slot '%s': %s"),
		*SlotName,
		bSaved ? TEXT("success") : TEXT("failed")
	);

	return bSaved;
}

bool USGameSaveSubsystem::BuildSaveData(FSGameSaveData& OutSaveData) const
{
	const UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSaveData failed: World is invalid."));
		return false;
	}

	const ASGameState* SGameState = World->GetGameState<ASGameState>();
	if (!IsValid(SGameState))
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSaveData failed: ASGameState is invalid."));
		return false;
	}

	const APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSaveData failed: PlayerController is invalid."));
		return false;
	}

	const ASCharacter* Character = Cast<ASCharacter>(PlayerController->GetPawn());
	if (!IsValid(Character))
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSaveData failed: Player pawn is not ASCharacter."));
		return false;
	}

	const USInventoryComponent* InventoryComponent = Character->GetInventoryComponent();
	if (!IsValid(InventoryComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSaveData failed: InventoryComponent is invalid."));
		return false;
	}

	const USCharacterSurvivalComponent* SurvivalComponent = Character->GetSurvivalComponent();
	if (!IsValid(SurvivalComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSaveData failed: SurvivalComponent is invalid."));
		return false;
	}

	OutSaveData.World.WorldTime = SGameState->GetWorldTime();
	OutSaveData.World.WorldTemperatureC = SGameState->GetWorldTemperature();

	OutSaveData.Player.Location = Character->GetActorLocation();
	OutSaveData.Player.ActorRotation = Character->GetActorRotation();
	OutSaveData.Player.ControlRotation = PlayerController->GetControlRotation();

	OutSaveData.Inventory = InventoryComponent->BuildSaveData();

	OutSaveData.CharacterSurvival.Vitals = SurvivalComponent->GetVitals();
	OutSaveData.CharacterSurvival.bDead = SurvivalComponent->IsDead();

	return true;
}

bool USGameSaveSubsystem::ApplySaveData(const FSGameSaveData& SaveData) const
{
    UWorld* World = GetWorld();
    if (!IsValid(World))
    {
        UE_LOG(LogTemp, Error, TEXT("ApplySaveData failed: World is invalid."));
        return false;
    }

    ASGameState* SGameState = World->GetGameState<ASGameState>();
    if (!IsValid(SGameState))
    {
        UE_LOG(LogTemp, Error, TEXT("ApplySaveData failed: ASGameState is invalid."));
        return false;
    }

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (!IsValid(PlayerController))
    {
        UE_LOG(LogTemp, Error, TEXT("ApplySaveData failed: PlayerController is invalid."));
        return false;
    }

    ASCharacter* Character = Cast<ASCharacter>(PlayerController->GetPawn());
    if (!IsValid(Character))
    {
        UE_LOG(LogTemp, Error, TEXT("ApplySaveData failed: Player pawn is not ASCharacter."));
        return false;
    }

    USInventoryComponent* InventoryComponent = Character->GetInventoryComponent();
    if (!IsValid(InventoryComponent))
    {
        UE_LOG(LogTemp, Error, TEXT("ApplySaveData failed: InventoryComponent is invalid."));
        return false;
    }

    USCharacterSurvivalComponent* SurvivalComponent = Character->GetSurvivalComponent();
    if (!IsValid(SurvivalComponent))
    {
        UE_LOG(LogTemp, Error, TEXT("ApplySaveData failed: SurvivalComponent is invalid."));
        return false;
    }

    const USGameInstance* SGameInstance = World->GetGameInstance<USGameInstance>();
    if (!IsValid(SGameInstance))
    {
        UE_LOG(LogTemp, Error, TEXT("ApplySaveData failed: USGameInstance is invalid."));
        return false;
    }

    SGameState->SetWorldTime(SaveData.World.WorldTime);
    SGameState->SetWorldTemperature(SaveData.World.WorldTemperatureC);

    Character->SetActorLocationAndRotation(
	    SaveData.Player.Location,
	    SaveData.Player.ActorRotation,
	    false,
	    nullptr,
	    ETeleportType::TeleportPhysics
    );

	PlayerController->SetControlRotation(SaveData.Player.ControlRotation);

	UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
	if (IsValid(MovementComponent))
	{
		MovementComponent->StopMovementImmediately();
	}

    InventoryComponent->RestoreFromSaveData(SaveData.Inventory, SGameInstance);
    SurvivalComponent->RestoreVitals(SaveData.CharacterSurvival.Vitals, SaveData.CharacterSurvival.bDead);

    return true;
}

USGameSaveObject* USGameSaveSubsystem::CreateSaveObject(
	const FString& SlotName,
	const FSGameSaveData& SaveData
) const
{
	USGameSaveObject* SaveObject = Cast<USGameSaveObject>(
		UGameplayStatics::CreateSaveGameObject(USGameSaveObject::StaticClass())
	);

	if (!IsValid(SaveObject))
	{
		UE_LOG(LogTemp, Error, TEXT("CreateSaveObject failed."));
		return nullptr;
	}

	SaveObject->SetSlotName(SlotName);
	SaveObject->GetMutableSaveData() = SaveData;

	return SaveObject;
}

bool USGameSaveSubsystem::LoadCurrentGame(const FString& SlotName)
{
	if (SlotName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadCurrentGame failed: SlotName is empty."));
		return false;
	}

	USaveGame* LoadedObject = UGameplayStatics::LoadGameFromSlot(SlotName, DefaultSaveUserIndex);
	USGameSaveObject* GameSaveObject = Cast<USGameSaveObject>(LoadedObject);

	if (!IsValid(GameSaveObject))
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("LoadCurrentGame failed: slot '%s' was not found or invalid."),
			*SlotName
		);

		return false;
	}

	const bool bApplied = ApplySaveData(GameSaveObject->GetSaveData());

	UE_LOG(
		LogTemp,
		Log,
		TEXT("LoadCurrentGame slot '%s': %s"),
		*SlotName,
		bApplied ? TEXT("success") : TEXT("failed")
	);

	return bApplied;
}
