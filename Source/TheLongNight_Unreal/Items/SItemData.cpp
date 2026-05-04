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
