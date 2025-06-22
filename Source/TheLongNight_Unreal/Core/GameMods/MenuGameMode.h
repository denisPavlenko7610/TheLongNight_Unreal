#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MenuGameMode.generated.h"

UCLASS()
class THELONGNIGHT_UNREAL_API AMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	AMenuGameMode();
	virtual void BeginPlay() override;
};