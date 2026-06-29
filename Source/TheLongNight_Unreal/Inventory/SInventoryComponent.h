#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/SInventoryViewData.h"
#include "Save/SSaveTypes.h"
#include "SInventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FSOnInventoryChanged);

class USGameInstance;
class USItemData;

USTRUCT()
struct THELONGNIGHT_UNREAL_API FSInventoryEntry
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<USItemData> ItemData = nullptr;

	UPROPERTY()
	int32 Quantity = 1;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class THELONGNIGHT_UNREAL_API USInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USInventoryComponent();

	bool AddItem(USItemData* ItemData, int32 Quantity = 1, bool bIgnoreWeightLimit = false);
	bool RemoveItem(USItemData* ItemData, int32 Quantity = 1);

	bool HasItem(USItemData* ItemData, int32 Quantity = 1) const;

	float GetCurrentWeightKg() const;
	float GetMaxWeightKg() const;
	int32 GetTotalItemCount() const;

	FSInventorySaveData BuildSaveData() const;
	void RestoreFromSaveData(const FSInventorySaveData& SaveData, const USGameInstance* GameInstance);
	void Clear();

	FSOnInventoryChanged OnInventoryChanged;

	const TArray<FSInventoryEntry>& GetItems() const;
	USItemData* FindItemById(FName ItemId) const;
	FSInventoryViewData BuildViewData() const;


private:
	UPROPERTY()
	TArray<FSInventoryEntry> Items;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Inventory", meta = (ClampMin = "0.0"))
	float MaxWeightKg = 30.0f;

	bool bSuppressInventoryChangedEvent = false;

	void BroadcastInventoryChanged();
};
