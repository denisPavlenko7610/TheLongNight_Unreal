#include "Items/SItemData.h"

FText USItemData::GetDisplayName() const
{
	return DisplayName;
}

ESItemType USItemData::GetItemType() const
{
	return ItemType;
}

float USItemData::GetWeightKg() const
{
	return WeightKg;
}

FName USItemData::GetItemId() const
{
	return ItemId;
}

bool USItemData::CanStack() const
{
	return bCanStack;
}

int32 USItemData::GetMaxStackSize() const
{
	return FMath::Max(1, MaxStackSize);
}
