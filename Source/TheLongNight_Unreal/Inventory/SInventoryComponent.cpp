#include "Inventory/SInventoryComponent.h"

#include "Items/SItemData.h"

USInventoryComponent::USInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool USInventoryComponent::AddItem(USItemData* ItemData, int32 Quantity)
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

    if (NewWeight > MaxWeightKg)
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

    return true;
}

bool USInventoryComponent::RemoveItem(USItemData* ItemData, int32 Quantity)
{
    if (!IsValid(ItemData) || Quantity <= 0)
    {
        return false;
    }

    for (int32 Index = 0; Index < Items.Num(); ++Index)
    {
        FSInventoryEntry& Entry = Items[Index];

        if (Entry.ItemData != ItemData)
        {
            continue;
        }

        if (Entry.Quantity < Quantity)
        {
            return false;
        }

        Entry.Quantity -= Quantity;

        if (Entry.Quantity <= 0)
        {
            Items.RemoveAt(Index);
        }

        return true;
    }

    return false;
}

bool USInventoryComponent::HasItem(USItemData* ItemData, int32 Quantity) const
{
    if (!IsValid(ItemData) || Quantity <= 0)
    {
        return false;
    }

    const FSInventoryEntry* Entry = FindEntry(ItemData);
    if (!Entry)
    {
        return false;
    }

    return Entry->Quantity >= Quantity;
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

const FSInventoryEntry* USInventoryComponent::FindEntry(USItemData* ItemData) const
{
    if (!IsValid(ItemData))
    {
        return nullptr;
    }

    for (const FSInventoryEntry& Entry : Items)
    {
        if (Entry.ItemData == ItemData)
        {
            return &Entry;
        }
    }

    return nullptr;
}
