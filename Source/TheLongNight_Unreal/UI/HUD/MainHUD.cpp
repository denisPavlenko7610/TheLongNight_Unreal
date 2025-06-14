#include "MainHUD.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	FString LevelName = GetWorld()->GetMapName();
	LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	const bool bIsMainMenu = LevelName == TEXT("MainMenu");
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PlayerController))
	{
		return;
	}

	if (!bIsMainMenu)
	{
		PlayerController->SetShowMouseCursor(false);
		PlayerController->SetInputMode(FInputModeGameOnly());
		return;
	}

	if (MainMenuWidgetClass)
	{
		if (UUserWidget* Menu = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass))
		{
			Menu->AddToViewport();
			
			PlayerController->SetShowMouseCursor(true);
			PlayerController->SetInputMode(FInputModeUIOnly());
		}
	}
}
