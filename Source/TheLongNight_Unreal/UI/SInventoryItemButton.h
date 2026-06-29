#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "SInventoryItemButton.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FSOnInventoryItemButtonClicked, FName);

UCLASS()
class THELONGNIGHT_UNREAL_API USInventoryItemButton : public UButton
{
	GENERATED_BODY()

public:
	void SetInventoryItemId(FName NewItemId);
	FName GetInventoryItemId() const;

	FSOnInventoryItemButtonClicked OnInventoryItemClicked;

private:
	UFUNCTION()
	void HandleClicked();

	FName ItemId;
	bool bClickHandlerBound = false;
};
