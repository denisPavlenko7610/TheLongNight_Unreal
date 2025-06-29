#include "MainHUD.h"

#include "HUDWidget.h"
#include "Blueprint/UserWidget.h"
#include "Character/PlayerInteractionComponent.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	AddHudWidget();
}

bool AMainHUD::AddHudWidget()
{
	if (!HUDWidgetClass)
	{
		return true;
	}
	
	HUDWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
	if (!HUDWidgetInstance)
	{
		return true;
	}
	
	HUDWidgetInstance->AddToViewport();

	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController || !PlayerController->GetPawn())
	{
		return true;
	}
	
	UPlayerInteractionComponent* InteractComponent =
		PlayerController->GetPawn()->FindComponentByClass<UPlayerInteractionComponent>();

	if (InteractComponent)
	{
		if (UHUDWidget* HudWidget = Cast<UHUDWidget>(HUDWidgetInstance))
		{
			InteractComponent->OnHoverEnter.AddDynamic(HudWidget, &UHUDWidget::OnHoverEnter_Item);
			InteractComponent->OnHoverExit .AddDynamic(HudWidget, &UHUDWidget::OnHoverExit_Item);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MainHUD: HUDWidgetInstance is not a UHUDWidget!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainHUD: Could not find PlayerInteractionComponent on pawn."));
	}
	return false;
}