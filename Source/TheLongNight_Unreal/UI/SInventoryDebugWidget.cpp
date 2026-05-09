#include "UI/SInventoryDebugWidget.h"

#include "Components/TextBlock.h"
#include "Inventory/SInventoryComponent.h"
#include "Items/SItemData.h"

void USInventoryDebugWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);
}

void USInventoryDebugWidget::RefreshFromInventory(const USInventoryComponent* InventoryComponent)
{
	if (!IsValid(InventoryTextBlock))
	{
		return;
	}

	if (!IsValid(InventoryComponent))
	{
		InventoryTextBlock->SetText(FText::FromString(TEXT("Inventory is invalid.")));
		return;
	}

	FString InventoryText;

	InventoryText += TEXT("INVENTORY\n");
	InventoryText += TEXT("--------------------\n");

	const TArray<FSInventoryEntry>& Items = InventoryComponent->GetItems();

	if (Items.Num() == 0)
	{
		InventoryText += TEXT("Empty\n");
	}
	else
	{
		for (const FSInventoryEntry& Entry : Items)
		{
			if (!IsValid(Entry.ItemData))
			{
				continue;
			}

			InventoryText += FString::Printf(
				TEXT("%s x%d  %.2f kg\n"),
				*Entry.ItemData->GetDisplayName().ToString(),
				Entry.Quantity,
				Entry.ItemData->GetWeightKg() * static_cast<float>(Entry.Quantity)
			);
		}
	}

	InventoryText += TEXT("--------------------\n");

	InventoryText += FString::Printf(
		TEXT("Weight: %.2f / %.2f kg\n"),
		InventoryComponent->GetCurrentWeightKg(),
		InventoryComponent->GetMaxWeightKg()
	);

	InventoryTextBlock->SetText(FText::FromString(InventoryText));
}
