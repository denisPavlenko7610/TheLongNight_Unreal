#include "UI/SInteractionPromptWidget.h"

#include "Components/TextBlock.h"

void USInteractionPromptWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HidePrompt();
}

void USInteractionPromptWidget::ShowPrompt(const FText& InteractionText)
{
	if (IsValid(InteractionTextBlock))
	{
		InteractionTextBlock->SetText(InteractionText);
	}

	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void USInteractionPromptWidget::HidePrompt()
{
	SetVisibility(ESlateVisibility::Hidden);
}
