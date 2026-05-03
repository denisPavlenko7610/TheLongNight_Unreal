#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

UCLASS()
class THELONGNIGHT_UNREAL_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASGameMode();

	virtual void BeginPlay() override;

	void StartWorld();
	void RequestGameOver();

private:
	void SetupNewWorld();
};
