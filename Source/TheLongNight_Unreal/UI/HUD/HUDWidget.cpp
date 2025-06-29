#include "HUDWidget.h"

#include "PickableItem.h"
#include "Data/PickableItemRow.h"
#include "Components/TextBlock.h"

void UHUDWidget::NativeConstruct()
{
	HideItem();
}

void UHUDWidget::OnHoverEnter_Item(APickableItem* PickedItem)
{
	if (PickedItem)
	{
		const FPickableItemRow& Row = PickedItem->GetDataRow();
		ShowItem(Row);
	}
}

void UHUDWidget::OnHoverExit_Item(APickableItem* PickedItem)
{
	HideItem();
}

void UHUDWidget::ShowItem(const FPickableItemRow& DataRow)
{
	if (ItemNameText)
	{
		ItemNameText->SetText(DataRow.ItemName);
		PlayAnimation(FadeInAnim);
	}
}

void UHUDWidget::HideItem()
{
	PlayAnimation(FadeOutAnim);
}