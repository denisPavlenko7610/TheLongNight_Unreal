#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SInventoryDebugWidget.generated.h"

class UTextBlock;
class USInventoryComponent;

UCLASS()
class THELONGNIGHT_UNREAL_API USInventoryDebugWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void RefreshFromInventory(const USInventoryComponent* InventoryComponent);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InventoryTextBlock;
};
