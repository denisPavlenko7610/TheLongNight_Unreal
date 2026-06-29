#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/SInventoryViewData.h"
#include "SInventoryItemRowWidget.generated.h"

class UButton;
class UTextBlock;
enum class ESItemType : uint8;

DECLARE_MULTICAST_DELEGATE_OneParam(FSOnInventoryItemRowClicked, FName);

UCLASS()
class THELONGNIGHT_UNREAL_API USInventoryItemRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetItemData(const FSInventoryItemViewData& ItemData, bool bSelected);
	void SetSelected(bool bSelected);

	FSOnInventoryItemRowClicked OnInventoryItemClicked;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void HandleRowClicked();

	static FText GetItemTypeText(ESItemType ItemType);
	static FText FormatWeight(float WeightKg);

private:
	FName ItemId;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RowButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TypeText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuantityText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WeightText;
};
