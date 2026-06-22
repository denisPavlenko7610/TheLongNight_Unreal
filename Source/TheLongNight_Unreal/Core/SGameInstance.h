#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Core/STypes.h"
#include "SGameInstance.generated.h"

class USItemRegistryData;
class USItemData;

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

private:
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
};
