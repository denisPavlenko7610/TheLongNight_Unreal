#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SInteractionPromptWidget.generated.h"

class UTextBlock;

UCLASS()
class THELONGNIGHT_UNREAL_API USInteractionPromptWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void ShowPrompt(const FText& InteractionText);
	void HidePrompt();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InteractionTextBlock;
};
