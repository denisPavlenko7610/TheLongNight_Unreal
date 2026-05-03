#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Core/STypes.h"
#include "ASGameState.generated.h"

UCLASS()
class THELONGNIGHT_UNREAL_API ASGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ASGameState();

	virtual void Tick(float DeltaSeconds) override;

	ESGamePhase GetGamePhase() const;
	const FSWorldTime& GetWorldTime() const;
	float GetWorldTemperature() const;

	void SetGamePhase(ESGamePhase NewGamePhase);
	void SetWorldTemperature(float NewWorldTemperature);

private:
	UPROPERTY()
	ESGamePhase GamePhase = ESGamePhase::None;

	UPROPERTY()
	FSWorldTime WorldTime;

	UPROPERTY()
	float WorldTemperature = -10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Time")
	float GameMinutesPerRealSecond = 4.0f;

	float TimeAccumulator = 0.0f;
};
