#include "Inventory/SInventoryComponent.h"

#include "Core/SGameInstance.h"
#include "Items/SItemData.h"

USInventoryComponent::USInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool USInventoryComponent::AddItem(USItemData* ItemData, int32 Quantity, bool bIgnoreWeightLimit)
{
    if (!IsValid(ItemData))
    {
        UE_LOG(LogTemp, Warning, TEXT("AddItem failed: ItemData is invalid."));
        return false;
    }

    if (Quantity <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("AddItem failed: Quantity must be positive."));
        return false;
    }

    const float AddedWeight = ItemData->GetWeightKg() * static_cast<float>(Quantity);
    const float NewWeight = GetCurrentWeightKg() + AddedWeight;

    if (!bIgnoreWeightLimit && NewWeight > MaxWeightKg)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("AddItem failed: inventory is too heavy. Current: %.2f, Added: %.2f, Max: %.2f"),
            GetCurrentWeightKg(),
            AddedWeight,
            MaxWeightKg
        );

        return false;
    }

    int32 RemainingQuantity = Quantity;

    if (ItemData->CanStack())
    {
        const int32 MaxStackSize = ItemData->GetMaxStackSize();

        for (FSInventoryEntry& Entry : Items)
        {
            if (Entry.ItemData != ItemData)
            {
                continue;
            }

            if (Entry.Quantity >= MaxStackSize)
            {
                continue;
            }

            const int32 FreeSpace = MaxStackSize - Entry.Quantity;
            const int32 QuantityToAdd = FMath::Min(FreeSpace, RemainingQuantity);

            Entry.Quantity += QuantityToAdd;
            RemainingQuantity -= QuantityToAdd;

            if (RemainingQuantity <= 0)
            {
                break;
            }
        }

        while (RemainingQuantity > 0)
        {
            FSInventoryEntry NewEntry;
            NewEntry.ItemData = ItemData;
            NewEntry.Quantity = FMath::Min(MaxStackSize, RemainingQuantity);

            Items.Add(NewEntry);

            RemainingQuantity -= NewEntry.Quantity;
        }
    }
    else
    {
        for (int32 Index = 0; Index < RemainingQuantity; ++Index)
        {
            FSInventoryEntry NewEntry;
            NewEntry.ItemData = ItemData;
            NewEntry.Quantity = 1;

            Items.Add(NewEntry);
        }
    }

	BroadcastInventoryChanged();

    return true;
}

bool USInventoryComponent::RemoveItem(USItemData* ItemData, int32 Quantity)
{
	if (!IsValid(ItemData) || Quantity <= 0)
	{
		return false;
	}

	if (!HasItem(ItemData, Quantity))
	{
		return false;
	}

	int32 RemainingQuantity = Quantity;

	for (int32 Index = Items.Num() - 1; Index >= 0 && RemainingQuantity > 0; --Index)
	{
		FSInventoryEntry& Entry = Items[Index];

		if (Entry.ItemData != ItemData)
		{
			continue;
		}

		const int32 QuantityToRemove = FMath::Min(Entry.Quantity, RemainingQuantity);

		Entry.Quantity -= QuantityToRemove;
		RemainingQuantity -= QuantityToRemove;

		if (Entry.Quantity <= 0)
		{
			Items.RemoveAt(Index);
		}
	}

	BroadcastInventoryChanged();

	return true;
}

bool USInventoryComponent::HasItem(USItemData* ItemData, int32 Quantity) const
{
	if (!IsValid(ItemData) || Quantity <= 0)
	{
		return false;
	}

	int32 TotalQuantity = 0;

	for (const FSInventoryEntry& Entry : Items)
	{
		if (Entry.ItemData != ItemData)
		{
			continue;
		}

		TotalQuantity += Entry.Quantity;

		if (TotalQuantity >= Quantity)
		{
			return true;
		}
	}

	return false;
}

float USInventoryComponent::GetCurrentWeightKg() const
{
    float TotalWeight = 0.0f;

    for (const FSInventoryEntry& Entry : Items)
    {
        if (!IsValid(Entry.ItemData))
        {
            continue;
        }

        TotalWeight += Entry.ItemData->GetWeightKg() * static_cast<float>(Entry.Quantity);
    }

    return TotalWeight;
}

float USInventoryComponent::GetMaxWeightKg() const
{
    return MaxWeightKg;
}

int32 USInventoryComponent::GetTotalItemCount() const
{
    int32 TotalCount = 0;

    for (const FSInventoryEntry& Entry : Items)
    {
        TotalCount += Entry.Quantity;
    }

    return TotalCount;
}

const TArray<FSInventoryEntry>& USInventoryComponent::GetItems() const {
    return Items;
}

FSInventorySaveData USInventoryComponent::BuildSaveData() const
{
    FSInventorySaveData SaveData;

    for (const FSInventoryEntry& Entry : Items)
    {
        if (!IsValid(Entry.ItemData))
        {
            continue;
        }

        if (Entry.Quantity <= 0)
        {
            continue;
        }

        FSInventorySaveEntry SaveEntry;
        SaveEntry.ItemId = Entry.ItemData->GetItemId();
        SaveEntry.Quantity = Entry.Quantity;

        SaveData.Items.Add(SaveEntry);
    }

    return SaveData;
}

void USInventoryComponent::RestoreFromSaveData(
	const FSInventorySaveData& SaveData,
	const USGameInstance* GameInstance
)
{
	bSuppressInventoryChangedEvent = true;

	Clear();

	if (!IsValid(GameInstance))
	{
		bSuppressInventoryChangedEvent = false;
		BroadcastInventoryChanged();

		UE_LOG(LogTemp, Error, TEXT("RestoreFromSaveData failed: GameInstance is invalid."));
		return;
	}

	for (const FSInventorySaveEntry& SaveEntry : SaveData.Items)
	{
		if (SaveEntry.ItemId.IsNone())
		{
			continue;
		}

		if (SaveEntry.Quantity <= 0)
		{
			continue;
		}

		USItemData* ItemData = GameInstance->FindItemById(SaveEntry.ItemId);
		if (!IsValid(ItemData))
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("RestoreFromSaveData skipped missing item: %s"),
				*SaveEntry.ItemId.ToString()
			);

			continue;
		}

		AddItem(ItemData, SaveEntry.Quantity, true);
	}

	bSuppressInventoryChangedEvent = false;
	BroadcastInventoryChanged();
}

FSInventoryViewData USInventoryComponent::BuildViewData() const
{
	FSInventoryViewData ViewData;

	ViewData.TotalItemCount = GetTotalItemCount();
	ViewData.CurrentWeightKg = GetCurrentWeightKg();
	ViewData.MaxWeightKg = GetMaxWeightKg();

	TMap<USItemData*, int32> AggregatedQuantities;
	TArray<USItemData*> OrderedItems;

	for (const FSInventoryEntry& Entry : Items)
	{
		if (!IsValid(Entry.ItemData))
		{
			continue;
		}

		if (Entry.Quantity <= 0)
		{
			continue;
		}

		USItemData* ItemData = Entry.ItemData.Get();

		if (!AggregatedQuantities.Contains(ItemData))
		{
			OrderedItems.Add(ItemData);
		}

		AggregatedQuantities.FindOrAdd(ItemData) += Entry.Quantity;
	}

	for (USItemData* ItemData : OrderedItems)
	{
		if (!IsValid(ItemData))
		{
			continue;
		}

		const int32* Quantity = AggregatedQuantities.Find(ItemData);
		if (!Quantity)
		{
			continue;
		}

		FSInventoryItemViewData ItemViewData;
		ItemViewData.ItemId = ItemData->GetItemId();
		ItemViewData.DisplayName = ItemData->GetDisplayName();
		ItemViewData.ItemType = ItemData->GetItemType();
		ItemViewData.Quantity = *Quantity;
		ItemViewData.UnitWeightKg = ItemData->GetWeightKg();
		ItemViewData.TotalWeightKg = ItemViewData.UnitWeightKg * static_cast<float>(ItemViewData.Quantity);

		ViewData.Items.Add(ItemViewData);
	}

	return ViewData;
}

void USInventoryComponent::Clear()
{
	if (Items.IsEmpty())
	{
		return;
	}

	Items.Reset();

	BroadcastInventoryChanged();
}

void USInventoryComponent::BroadcastInventoryChanged()
{
	if (bSuppressInventoryChangedEvent)
	{
		return;
	}

	OnInventoryChanged.Broadcast();
}
