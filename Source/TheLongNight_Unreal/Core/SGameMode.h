#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

class USWorldDefinitionData;

UCLASS()
class THELONGNIGHT_UNREAL_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASGameMode();

	void StartWorld();
	void RequestGameOver();

protected:
	virtual void BeginPlay() override;

private:
	void SetupNewWorld();

	UPROPERTY(EditDefaultsOnly, Category = "Survival|World")
	TObjectPtr<USWorldDefinitionData> WorldDefinitionData;
};
