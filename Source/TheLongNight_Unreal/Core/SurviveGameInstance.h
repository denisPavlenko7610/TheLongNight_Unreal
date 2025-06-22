#pragma once

#include "Engine/GameInstance.h"
#include "SurviveGameInstance.generated.h"

UCLASS()
class USurviveGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Pause")
	TSubclassOf<UUserWidget> PauseMenuClass;
};
