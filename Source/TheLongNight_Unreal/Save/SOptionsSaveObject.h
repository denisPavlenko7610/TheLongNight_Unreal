#pragma once

#include "CoreMinimal.h"
#include "Core/SGameInstance.h"
#include "GameFramework/SaveGame.h"
#include "SOptionsSaveObject.generated.h"

UCLASS()
class THELONGNIGHT_UNREAL_API USOptionsSaveObject : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Survival|Options")
	FSGameOptionsSettings Settings;
};
