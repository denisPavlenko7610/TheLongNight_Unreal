#include "UI/SInventoryMenuWidget.h"

#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Core/SPlayerController.h"
#include "Inventory/SInventoryComponent.h"
#include "UI/SInventoryItemRowWidget.h"

namespace
{
	const FLinearColor InventoryAccentColor(0.58f, 0.74f, 0.82f, 1.0f);
	const FLinearColor InventoryDangerColor(0.80f, 0.24f, 0.18f, 1.0f);
}

void USInventoryMenuWidget::OpenForInventory(USInventoryComponent* InInventoryComponent)
{
	SetInventoryComponent(InInventoryComponent);
	UpdateInventoryView();

	if (!IsInViewport())
	{
		AddToViewport(75);
	}

	SetVisibility(ESlateVisibility::Visible);
}

void USInventoryMenuWidget::CloseInventory()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

bool USInventoryMenuWidget::IsInventoryOpen() const
{
	return GetVisibility() == ESlateVisibility::Visible;
}

void USInventoryMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	EnsureItemRowWidgetClass();
	BindEvents();
	SetVisibility(ESlateVisibility::Collapsed);
}

void USInventoryMenuWidget::NativeDestruct()
{
	UnbindEvents();

	if (IsValid(InventoryComponent))
	{
		InventoryComponent->OnInventoryChanged.RemoveAll(this);
	}

	Super::NativeDestruct();
}

void USInventoryMenuWidget::BindEvents()
{
	UnbindEvents();

	if (IsValid(AllFilterButton))
	{
		AllFilterButton->OnClicked.AddDynamic(this, &USInventoryMenuWidget::HandleAllFilterClicked);
	}

	if (IsValid(FoodFilterButton))
	{
		FoodFilterButton->OnClicked.AddDynamic(this, &USInventoryMenuWidget::HandleFoodFilterClicked);
	}

	if (IsValid(WaterFilterButton))
	{
		WaterFilterButton->OnClicked.AddDynamic(this, &USInventoryMenuWidget::HandleWaterFilterClicked);
	}

	if (IsValid(ToolFilterButton))
	{
		ToolFilterButton->OnClicked.AddDynamic(this, &USInventoryMenuWidget::HandleToolFilterClicked);
	}

	if (IsValid(ClothingFilterButton))
	{
		ClothingFilterButton->OnClicked.AddDynamic(this, &USInventoryMenuWidget::HandleClothingFilterClicked);
	}

	if (IsValid(WeaponFilterButton))
	{
		WeaponFilterButton->OnClicked.AddDynamic(this, &USInventoryMenuWidget::HandleWeaponFilterClicked);
	}

	if (IsValid(MedicineFilterButton))
	{
		MedicineFilterButton->OnClicked.AddDynamic(this, &USInventoryMenuWidget::HandleMedicineFilterClicked);
	}

	if (IsValid(FuelFilterButton))
	{
		FuelFilterButton->OnClicked.AddDynamic(this, &USInventoryMenuWidget::HandleFuelFilterClicked);
	}

	if (IsValid(DropButton))
	{
		DropButton->OnClicked.AddDynamic(this, &USInventoryMenuWidget::HandleDropClicked);
	}

	if (IsValid(CloseButton))
	{
		CloseButton->OnClicked.AddDynamic(this, &USInventoryMenuWidget::HandleCloseClicked);
	}
}

void USInventoryMenuWidget::UnbindEvents()
{
	if (IsValid(AllFilterButton))
	{
		AllFilterButton->OnClicked.RemoveDynamic(this, &USInventoryMenuWidget::HandleAllFilterClicked);
	}

	if (IsValid(FoodFilterButton))
	{
		FoodFilterButton->OnClicked.RemoveDynamic(this, &USInventoryMenuWidget::HandleFoodFilterClicked);
	}

	if (IsValid(WaterFilterButton))
	{
		WaterFilterButton->OnClicked.RemoveDynamic(this, &USInventoryMenuWidget::HandleWaterFilterClicked);
	}

	if (IsValid(ToolFilterButton))
	{
		ToolFilterButton->OnClicked.RemoveDynamic(this, &USInventoryMenuWidget::HandleToolFilterClicked);
	}

	if (IsValid(ClothingFilterButton))
	{
		ClothingFilterButton->OnClicked.RemoveDynamic(this, &USInventoryMenuWidget::HandleClothingFilterClicked);
	}

	if (IsValid(WeaponFilterButton))
	{
		WeaponFilterButton->OnClicked.RemoveDynamic(this, &USInventoryMenuWidget::HandleWeaponFilterClicked);
	}

	if (IsValid(MedicineFilterButton))
	{
		MedicineFilterButton->OnClicked.RemoveDynamic(this, &USInventoryMenuWidget::HandleMedicineFilterClicked);
	}

	if (IsValid(FuelFilterButton))
	{
		FuelFilterButton->OnClicked.RemoveDynamic(this, &USInventoryMenuWidget::HandleFuelFilterClicked);
	}

	if (IsValid(DropButton))
	{
		DropButton->OnClicked.RemoveDynamic(this, &USInventoryMenuWidget::HandleDropClicked);
	}

	if (IsValid(CloseButton))
	{
		CloseButton->OnClicked.RemoveDynamic(this, &USInventoryMenuWidget::HandleCloseClicked);
	}
}

void USInventoryMenuWidget::EnsureItemRowWidgetClass()
{
	if (IsValid(ItemRowWidgetClass))
	{
		return;
	}

	ItemRowWidgetClass = LoadClass<USInventoryItemRowWidget>(
		nullptr,
		TEXT("/Game/UI/BP_InventoryItemRow.BP_InventoryItemRow_C")
	);
}

void USInventoryMenuWidget::SetInventoryComponent(USInventoryComponent* InInventoryComponent)
{
	if (InventoryComponent == InInventoryComponent)
	{
		return;
	}

	if (IsValid(InventoryComponent))
	{
		InventoryComponent->OnInventoryChanged.RemoveAll(this);
	}

	InventoryComponent = InInventoryComponent;

	if (IsValid(InventoryComponent))
	{
		InventoryComponent->OnInventoryChanged.AddUObject(this, &USInventoryMenuWidget::HandleInventoryChanged);
	}
}

void USInventoryMenuWidget::UpdateInventoryView()
{
	if (!IsValid(InventoryComponent))
	{
		if (IsValid(ItemListBox))
		{
			ItemListBox->ClearChildren();
		}

		if (IsValid(EmptyListText))
		{
			EmptyListText->SetVisibility(ESlateVisibility::Visible);
		}

		SelectedItemId = NAME_None;
		return;
	}

	const FSInventoryViewData ViewData = InventoryComponent->BuildViewData();
	RefreshWeightSummary(ViewData);

	TArray<FSInventoryItemViewData> FilteredItems;
	for (const FSInventoryItemViewData& Item : ViewData.Items)
	{
		if (PassesActiveFilter(Item))
		{
			FilteredItems.Add(Item);
		}
	}

	FilteredItems.Sort([](const FSInventoryItemViewData& Left, const FSInventoryItemViewData& Right)
	{
		if (Left.ItemType != Right.ItemType)
		{
			return static_cast<uint8>(Left.ItemType) < static_cast<uint8>(Right.ItemType);
		}

		return Left.DisplayName.ToString() < Right.DisplayName.ToString();
	});

	RefreshItemList(FilteredItems);
	RefreshSelectionDetails(FilteredItems);
}

void USInventoryMenuWidget::RefreshItemList(const TArray<FSInventoryItemViewData>& Items)
{
	if (!IsValid(ItemListBox))
	{
		return;
	}

	ItemListBox->ClearChildren();

	bool bSelectionStillVisible = false;
	for (const FSInventoryItemViewData& Item : Items)
	{
		if (Item.ItemId == SelectedItemId)
		{
			bSelectionStillVisible = true;
			break;
		}
	}

	if (!bSelectionStillVisible)
	{
		SelectedItemId = Items.IsEmpty() ? NAME_None : Items[0].ItemId;
	}

	if (IsValid(EmptyListText))
	{
		EmptyListText->SetVisibility(Items.IsEmpty() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (!IsValid(ItemRowWidgetClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory item row widget class is not assigned."));
		return;
	}

	for (const FSInventoryItemViewData& Item : Items)
	{
		USInventoryItemRowWidget* ItemRow = CreateWidget<USInventoryItemRowWidget>(
			GetOwningPlayer(),
			ItemRowWidgetClass
		);
		if (!IsValid(ItemRow))
		{
			continue;
		}

		ItemRow->SetItemData(Item, Item.ItemId == SelectedItemId);
		ItemRow->OnInventoryItemClicked.AddUObject(this, &USInventoryMenuWidget::HandleItemClicked);

		UVerticalBoxSlot* RowSlot = ItemListBox->AddChildToVerticalBox(ItemRow);
		if (IsValid(RowSlot))
		{
			RowSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 8.0f));
		}
	}
}

void USInventoryMenuWidget::RefreshSelectionDetails(const TArray<FSInventoryItemViewData>& Items)
{
	const FSInventoryItemViewData* SelectedItem = nullptr;
	for (const FSInventoryItemViewData& Item : Items)
	{
		if (Item.ItemId == SelectedItemId)
		{
			SelectedItem = &Item;
			break;
		}
	}

	const bool bHasSelection = SelectedItem != nullptr;

	if (IsValid(DropButton))
	{
		DropButton->SetIsEnabled(bHasSelection);
	}

	if (!bHasSelection)
	{
		if (IsValid(SelectedNameText))
		{
			SelectedNameText->SetText(FText::FromString(TEXT("No item selected")));
		}

		if (IsValid(SelectedTypeText))
		{
			SelectedTypeText->SetText(FText::GetEmpty());
		}

		if (IsValid(SelectedDescriptionText))
		{
			SelectedDescriptionText->SetText(FText::FromString(TEXT("Backpack is empty.")));
		}

		if (IsValid(SelectedQuantityText))
		{
			SelectedQuantityText->SetText(FText::GetEmpty());
		}

		if (IsValid(SelectedUnitWeightText))
		{
			SelectedUnitWeightText->SetText(FText::GetEmpty());
		}

		if (IsValid(SelectedTotalWeightText))
		{
			SelectedTotalWeightText->SetText(FText::GetEmpty());
		}

		return;
	}

	if (IsValid(SelectedNameText))
	{
		SelectedNameText->SetText(SelectedItem->DisplayName);
	}

	if (IsValid(SelectedTypeText))
	{
		SelectedTypeText->SetText(GetItemTypeText(SelectedItem->ItemType));
	}

	if (IsValid(SelectedDescriptionText))
	{
		SelectedDescriptionText->SetText(
			SelectedItem->Description.IsEmpty()
				? FText::FromString(TEXT("No field notes recorded."))
				: SelectedItem->Description
		);
	}

	if (IsValid(SelectedQuantityText))
	{
		SelectedQuantityText->SetText(FText::FromString(FString::Printf(TEXT("Quantity: %d"), SelectedItem->Quantity)));
	}

	if (IsValid(SelectedUnitWeightText))
	{
		SelectedUnitWeightText->SetText(FText::Format(FText::FromString(TEXT("Unit Weight: {0}")), FormatWeight(SelectedItem->UnitWeightKg)));
	}

	if (IsValid(SelectedTotalWeightText))
	{
		SelectedTotalWeightText->SetText(FText::Format(FText::FromString(TEXT("Total Weight: {0}")), FormatWeight(SelectedItem->TotalWeightKg)));
	}
}

void USInventoryMenuWidget::RefreshWeightSummary(const FSInventoryViewData& ViewData) const
{
	if (IsValid(SummaryText))
	{
		SummaryText->SetText(FText::FromString(FString::Printf(TEXT("%d ITEMS"), ViewData.TotalItemCount)));
	}

	if (IsValid(WeightText))
	{
		WeightText->SetText(FText::FromString(FString::Printf(
			TEXT("%.1f / %.1f KG"),
			ViewData.CurrentWeightKg,
			ViewData.MaxWeightKg
		)));
	}

	if (IsValid(WeightProgressBar))
	{
		const float WeightPercent = ViewData.MaxWeightKg > 0.0f
			? FMath::Clamp(ViewData.CurrentWeightKg / ViewData.MaxWeightKg, 0.0f, 1.0f)
			: 0.0f;

		WeightProgressBar->SetPercent(WeightPercent);
		WeightProgressBar->SetFillColorAndOpacity(WeightPercent > 0.85f ? InventoryDangerColor : InventoryAccentColor);
	}
}

bool USInventoryMenuWidget::PassesActiveFilter(const FSInventoryItemViewData& Item) const
{
	if (!bUseTypeFilter)
	{
		return true;
	}

	return Item.ItemType == ActiveFilter;
}

FText USInventoryMenuWidget::GetItemTypeText(ESItemType ItemType)
{
	switch (ItemType)
	{
	case ESItemType::Food:
		return FText::FromString(TEXT("Food"));
	case ESItemType::Water:
		return FText::FromString(TEXT("Water"));
	case ESItemType::Tool:
		return FText::FromString(TEXT("Tool"));
	case ESItemType::Clothing:
		return FText::FromString(TEXT("Clothing"));
	case ESItemType::Weapon:
		return FText::FromString(TEXT("Weapon"));
	case ESItemType::Medicine:
		return FText::FromString(TEXT("Medicine"));
	case ESItemType::Fuel:
		return FText::FromString(TEXT("Fuel"));
	case ESItemType::Misc:
	default:
		return FText::FromString(TEXT("Misc"));
	}
}

FText USInventoryMenuWidget::FormatWeight(float WeightKg)
{
	return FText::FromString(FString::Printf(TEXT("%.2f KG"), WeightKg));
}

void USInventoryMenuWidget::SetAllFilter()
{
	bUseTypeFilter = false;
	UpdateInventoryView();
}

void USInventoryMenuWidget::SetTypeFilter(ESItemType ItemType)
{
	bUseTypeFilter = true;
	ActiveFilter = ItemType;
	UpdateInventoryView();
}

void USInventoryMenuWidget::HandleAllFilterClicked()
{
	SetAllFilter();
}

void USInventoryMenuWidget::HandleFoodFilterClicked()
{
	SetTypeFilter(ESItemType::Food);
}

void USInventoryMenuWidget::HandleWaterFilterClicked()
{
	SetTypeFilter(ESItemType::Water);
}

void USInventoryMenuWidget::HandleToolFilterClicked()
{
	SetTypeFilter(ESItemType::Tool);
}

void USInventoryMenuWidget::HandleClothingFilterClicked()
{
	SetTypeFilter(ESItemType::Clothing);
}

void USInventoryMenuWidget::HandleWeaponFilterClicked()
{
	SetTypeFilter(ESItemType::Weapon);
}

void USInventoryMenuWidget::HandleMedicineFilterClicked()
{
	SetTypeFilter(ESItemType::Medicine);
}

void USInventoryMenuWidget::HandleFuelFilterClicked()
{
	SetTypeFilter(ESItemType::Fuel);
}

void USInventoryMenuWidget::HandleDropClicked()
{
	if (!IsValid(InventoryComponent) || SelectedItemId.IsNone())
	{
		return;
	}

	USItemData* ItemData = InventoryComponent->FindItemById(SelectedItemId);
	if (!IsValid(ItemData))
	{
		return;
	}

	InventoryComponent->RemoveItem(ItemData, 1);
}

void USInventoryMenuWidget::HandleCloseClicked()
{
	ASPlayerController* SPlayerController = Cast<ASPlayerController>(GetOwningPlayer());
	if (IsValid(SPlayerController))
	{
		SPlayerController->CloseInventory();
		return;
	}

	CloseInventory();
}

void USInventoryMenuWidget::HandleInventoryChanged()
{
	UpdateInventoryView();
}

void USInventoryMenuWidget::HandleItemClicked(FName ItemId)
{
	SelectedItemId = ItemId;
	UpdateInventoryView();
}
