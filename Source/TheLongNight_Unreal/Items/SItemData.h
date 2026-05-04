#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SItemData.generated.h"

UENUM()
enum class ESItemType : uint8
{
	Misc,
	Food,
	Water,
	Tool,
	Clothing,
	Weapon,
	Medicine,
	Fuel
};

UCLASS()
class THELONGNIGHT_UNREAL_API USItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	FText GetDisplayName() const;
	ESItemType GetItemType() const;
	float GetWeightKg() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Survival|Item")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Item")
	ESItemType ItemType = ESItemType::Misc;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Item", meta = (ClampMin = "0.0"))
	float WeightKg = 0.1f;
};
