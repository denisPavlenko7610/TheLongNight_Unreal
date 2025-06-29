#include "PickableItemManager.h"
#include "PickableItemRow.h"

bool UPickableItemManager::GetItemDataByRowName(FName RowName, FPickableItemRow& OutRow) const
{
	if (!ItemTable)
		return false;
	
	static const FString Context = TEXT("GetPickableItemData");
	const FPickableItemRow* Row = ItemTable->FindRow<FPickableItemRow>(RowName, Context);
	if (!Row)
		return false;
	
	OutRow = *Row;
	return true;
}