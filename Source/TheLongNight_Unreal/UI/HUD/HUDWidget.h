#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class APickableItem;
struct FPickableItemRow;
class UTextBlock;

UCLASS()
class THELONGNIGHT_UNREAL_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemNameText;

	UFUNCTION()
	void OnHoverEnter_Item(APickableItem* PickedItem);

	UFUNCTION()
	void OnHoverExit_Item(APickableItem* PickedItem);
	
	UFUNCTION(BlueprintCallable)
	void ShowItem(const FPickableItemRow& DataRow);

	UFUNCTION(BlueprintCallable)
	void HideItem();

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* FadeInAnim;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* FadeOutAnim;
};
