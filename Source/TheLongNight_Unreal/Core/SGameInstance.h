#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Core/STypes.h"
#include "SGameInstance.generated.h"

class USItemRegistryData;
class USItemData;
class USGameSaveSubsystem;

USTRUCT(BlueprintType)
struct FSGameOptionsSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Survival|Options")
	float MasterVolume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Survival|Options")
	float MusicVolume = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Survival|Options")
	float SfxVolume = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Survival|Options")
	float MouseSensitivity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Survival|Options")
	bool bInvertMouseY = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Survival|Options")
	bool bHoldToSprint = true;
};

UCLASS()
class THELONGNIGHT_UNREAL_API USGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	USItemData* FindItemById(FName ItemId) const;

	UFUNCTION(BlueprintCallable, Category = "Survival|Flow")
	void StartNewGame();

	UFUNCTION(BlueprintCallable, Category = "Survival|Flow")
	void LoadGame();

	UFUNCTION(BlueprintCallable, Category = "Survival|Flow")
	void ReturnToMainMenu();

	UFUNCTION(BlueprintCallable, Category = "Survival|Save")
	void SetActiveSaveSlotName(const FString& NewSlotName);

	UFUNCTION(BlueprintPure, Category = "Survival|Save")
	FString GetActiveSaveSlotName() const;

	UFUNCTION(BlueprintPure, Category = "Survival|Flow")
	ESTypes GetStartGameMode() const;

	UFUNCTION(BlueprintCallable, Category = "Survival|Save")
	bool SaveActiveSlot();

	UFUNCTION(BlueprintCallable, Category = "Survival|Save")
	bool LoadActiveSlotInCurrentWorld();

	const FSGameOptionsSettings& GetOptionsSettings() const;

	UFUNCTION(BlueprintPure, Category = "Survival|Options")
	FSGameOptionsSettings GetOptionsSettingsCopy() const;

	UFUNCTION(BlueprintCallable, Category = "Survival|Options")
	void SetOptionsSettings(const FSGameOptionsSettings& NewSettings, bool bSaveImmediately = true);

	UFUNCTION(BlueprintCallable, Category = "Survival|Options")
	void LoadOptionsSettings();

	UFUNCTION(BlueprintCallable, Category = "Survival|Options")
	bool SaveOptionsSettings() const;

	UFUNCTION(BlueprintCallable, Category = "Survival|Options")
	void ApplyCustomOptionsSettings() const;

private:
	static FString GetOptionsSaveSlotName();

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Maps")
	FName MainMenuMapName = TEXT("L_MainMenu");

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Maps")
	FName OpenWorldMapName = TEXT("L_OpenWorld");

	UPROPERTY(VisibleInstanceOnly, Category = "Survival|Save")
	FString ActiveSaveSlotName = TEXT("Slot_01");

	UPROPERTY(VisibleInstanceOnly, Category = "Survival|Flow")
	ESTypes StartGameMode = ESTypes::NewGame;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Items")
	TObjectPtr<USItemRegistryData> ItemRegistryData;

	UPROPERTY(VisibleInstanceOnly, Category = "Survival|Options")
	FSGameOptionsSettings OptionsSettings;
};
