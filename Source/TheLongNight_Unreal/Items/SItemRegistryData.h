#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SItemRegistryData.generated.h"

class USItemData;

UCLASS()
class THELONGNIGHT_UNREAL_API USItemRegistryData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	USItemData* FindItemById(FName ItemId) const;

	const TArray<TObjectPtr<USItemData>>& GetItems() const
	{
		return Items;
	}

	#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif

private:
	UPROPERTY(EditDefaultsOnly, Category = "Survival|Items")
	TArray<TObjectPtr<USItemData>> Items;

	void ValidateItems() const;
};
