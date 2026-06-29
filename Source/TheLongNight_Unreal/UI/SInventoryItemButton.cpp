#include "UI/SInventoryItemButton.h"

void USInventoryItemButton::SetInventoryItemId(FName NewItemId)
{
	ItemId = NewItemId;

	if (!bClickHandlerBound)
	{
		OnClicked.AddDynamic(this, &USInventoryItemButton::HandleClicked);
		bClickHandlerBound = true;
	}
}

FName USInventoryItemButton::GetInventoryItemId() const
{
	return ItemId;
}

void USInventoryItemButton::HandleClicked()
{
	OnInventoryItemClicked.Broadcast(ItemId);
}
