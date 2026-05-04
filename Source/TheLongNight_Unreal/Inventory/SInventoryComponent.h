#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInventoryComponent.generated.h"

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

	bool AddItem(USItemData* ItemData, int32 Quantity = 1);
	bool RemoveItem(USItemData* ItemData, int32 Quantity = 1);

	bool HasItem(USItemData* ItemData, int32 Quantity = 1) const;

	float GetCurrentWeightKg() const;
	float GetMaxWeightKg() const;
	int32 GetTotalItemCount() const;

private:
	FSInventoryEntry* FindEntryMutable(USItemData* ItemData);
	const FSInventoryEntry* FindEntry(USItemData* ItemData) const;

private:
	UPROPERTY()
	TArray<FSInventoryEntry> Items;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Inventory", meta = (ClampMin = "0.0"))
	float MaxWeightKg = 30.0f;
};
