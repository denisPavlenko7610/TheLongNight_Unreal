#pragma once

#include "CoreMinimal.h"
#include "Items/SItemData.h"
#include "SInventoryViewData.generated.h"

USTRUCT(BlueprintType)
struct THELONGNIGHT_UNREAL_API FSInventoryItemViewData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Survival|Inventory")
	FName ItemId;

	UPROPERTY(BlueprintReadOnly, Category = "Survival|Inventory")
	FText DisplayName;

	UPROPERTY(BlueprintReadOnly, Category = "Survival|Inventory")
	FText Description;

	UPROPERTY(BlueprintReadOnly, Category = "Survival|Inventory")
	ESItemType ItemType = ESItemType::Misc;

	UPROPERTY(BlueprintReadOnly, Category = "Survival|Inventory")
	int32 Quantity = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Survival|Inventory")
	float UnitWeightKg = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Survival|Inventory")
	float TotalWeightKg = 0.0f;
};

USTRUCT(BlueprintType)
struct THELONGNIGHT_UNREAL_API FSInventoryViewData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Survival|Inventory")
	TArray<FSInventoryItemViewData> Items;

	UPROPERTY(BlueprintReadOnly, Category = "Survival|Inventory")
	int32 TotalItemCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Survival|Inventory")
	float CurrentWeightKg = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Survival|Inventory")
	float MaxWeightKg = 0.0f;
};
