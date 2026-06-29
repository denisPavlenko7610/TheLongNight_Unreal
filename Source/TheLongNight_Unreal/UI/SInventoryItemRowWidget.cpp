#include "UI/SInventoryItemRowWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Items/SItemData.h"

namespace
{
	const FLinearColor InventoryButtonColor(0.09f, 0.12f, 0.14f, 1.0f);
	const FLinearColor InventorySelectedButtonColor(0.16f, 0.22f, 0.25f, 1.0f);
}

void USInventoryItemRowWidget::SetItemData(const FSInventoryItemViewData& ItemData, bool bSelected)
{
	ItemId = ItemData.ItemId;

	if (IsValid(NameText))
	{
		NameText->SetText(ItemData.DisplayName);
	}

	if (IsValid(TypeText))
	{
		TypeText->SetText(GetItemTypeText(ItemData.ItemType));
	}

	if (IsValid(QuantityText))
	{
		QuantityText->SetText(FText::FromString(FString::Printf(TEXT("x%d"), ItemData.Quantity)));
	}

	if (IsValid(WeightText))
	{
		WeightText->SetText(FormatWeight(ItemData.TotalWeightKg));
	}

	SetSelected(bSelected);
}

void USInventoryItemRowWidget::SetSelected(bool bSelected)
{
	if (IsValid(RowButton))
	{
		RowButton->SetBackgroundColor(bSelected ? InventorySelectedButtonColor : InventoryButtonColor);
	}
}

void USInventoryItemRowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(RowButton))
	{
		RowButton->OnClicked.RemoveDynamic(this, &USInventoryItemRowWidget::HandleRowClicked);
		RowButton->OnClicked.AddDynamic(this, &USInventoryItemRowWidget::HandleRowClicked);
	}
}

void USInventoryItemRowWidget::NativeDestruct()
{
	if (IsValid(RowButton))
	{
		RowButton->OnClicked.RemoveDynamic(this, &USInventoryItemRowWidget::HandleRowClicked);
	}

	Super::NativeDestruct();
}

void USInventoryItemRowWidget::HandleRowClicked()
{
	OnInventoryItemClicked.Broadcast(ItemId);
}

FText USInventoryItemRowWidget::GetItemTypeText(ESItemType ItemType)
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

FText USInventoryItemRowWidget::FormatWeight(float WeightKg)
{
	return FText::FromString(FString::Printf(TEXT("%.2f KG"), WeightKg));
}
