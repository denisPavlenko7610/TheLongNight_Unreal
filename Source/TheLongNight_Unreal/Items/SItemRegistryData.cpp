#include "Items/SItemRegistryData.h"

#include "Items/SItemData.h"

USItemData* USItemRegistryData::FindItemById(FName ItemId) const
{
	if (ItemId.IsNone())
	{
		return nullptr;
	}

	for (USItemData* ItemData : Items)
	{
		if (!IsValid(ItemData))
		{
			continue;
		}

		if (ItemData->GetItemId() == ItemId)
		{
			return ItemData;
		}
	}

	return nullptr;
}

void USItemRegistryData::ValidateItems() const
{
	TSet<FName> UsedIds;

	for (USItemData* ItemData : Items)
	{
		if (!IsValid(ItemData))
		{
			continue;
		}

		const FName ItemId = ItemData->GetItemId();

		if (ItemId.IsNone())
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("ItemRegistry contains item with empty ItemId: %s"),
				*ItemData->GetName()
			);

			continue;
		}

		if (UsedIds.Contains(ItemId))
		{
			UE_LOG(
				LogTemp,
				Error,
				TEXT("Duplicate ItemId in ItemRegistry: %s"),
				*ItemId.ToString()
			);

			continue;
		}

		UsedIds.Add(ItemId);
	}
}

#if WITH_EDITOR
void USItemRegistryData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	ValidateItems();
}
#endif
