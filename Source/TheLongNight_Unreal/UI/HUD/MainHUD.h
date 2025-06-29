#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

UCLASS()
class THELONGNIGHT_UNREAL_API AMainHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

protected:
	bool AddHudWidget();
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UUserWidget* HUDWidgetInstance;
};
