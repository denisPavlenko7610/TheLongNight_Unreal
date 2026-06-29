#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/SItemData.h"
#include "SInventoryMenuWidget.generated.h"

class UButton;
class UProgressBar;
class UTextBlock;
class USInventoryComponent;
class USInventoryItemRowWidget;
class UVerticalBox;
struct FSInventoryItemViewData;
struct FSInventoryViewData;

UCLASS()
class THELONGNIGHT_UNREAL_API USInventoryMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void OpenForInventory(USInventoryComponent* InInventoryComponent);
	void CloseInventory();
	bool IsInventoryOpen() const;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void BindEvents();
	void UnbindEvents();
	void EnsureItemRowWidgetClass();
	void SetInventoryComponent(USInventoryComponent* InInventoryComponent);

	void UpdateInventoryView();
	void RefreshItemList(const TArray<FSInventoryItemViewData>& Items);
	void RefreshSelectionDetails(const TArray<FSInventoryItemViewData>& Items);
	void RefreshWeightSummary(const FSInventoryViewData& ViewData) const;
	bool PassesActiveFilter(const FSInventoryItemViewData& Item) const;

	static FText GetItemTypeText(ESItemType ItemType);
	static FText FormatWeight(float WeightKg);

	void SetAllFilter();
	void SetTypeFilter(ESItemType ItemType);

	UFUNCTION()
	void HandleAllFilterClicked();

	UFUNCTION()
	void HandleFoodFilterClicked();

	UFUNCTION()
	void HandleWaterFilterClicked();

	UFUNCTION()
	void HandleToolFilterClicked();

	UFUNCTION()
	void HandleClothingFilterClicked();

	UFUNCTION()
	void HandleWeaponFilterClicked();

	UFUNCTION()
	void HandleMedicineFilterClicked();

	UFUNCTION()
	void HandleFuelFilterClicked();

	UFUNCTION()
	void HandleDropClicked();

	UFUNCTION()
	void HandleCloseClicked();

	void HandleInventoryChanged();
	void HandleItemClicked(FName ItemId);

private:
	bool bUseTypeFilter = false;
	ESItemType ActiveFilter = ESItemType::Misc;
	FName SelectedItemId;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Inventory")
	TSubclassOf<USInventoryItemRowWidget> ItemRowWidgetClass;

	UPROPERTY()
	TObjectPtr<USInventoryComponent> InventoryComponent;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SummaryText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WeightText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> WeightProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> ItemListBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EmptyListText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SelectedNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SelectedTypeText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SelectedDescriptionText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SelectedQuantityText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SelectedUnitWeightText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SelectedTotalWeightText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> AllFilterButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> FoodFilterButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> WaterFilterButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ToolFilterButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ClothingFilterButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> WeaponFilterButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> MedicineFilterButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> FuelFilterButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> DropButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;
};
