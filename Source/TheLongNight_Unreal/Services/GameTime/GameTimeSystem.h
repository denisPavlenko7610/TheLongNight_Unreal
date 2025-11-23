#pragma once

#include "CoreMinimal.h"
#include "GameTimeSystem.generated.h"

USTRUCT(BlueprintType)
struct FGameTimeSystem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Day = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Hour = 8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Minute = 0;

	void AddMinutes(int32 minutesToAdd) {
		int32 totalMinutes = (Day - 1) * 24 * 60 + Hour * 60 + Minute + minutesToAdd;
		if (totalMinutes < 0) {
			totalMinutes = 0;
		}

		Day = totalMinutes / (24 * 60) + 1;
		int32 minutesSinceMidnight = totalMinutes % (24 * 60);
		Hour = minutesSinceMidnight / 60;
		Minute = minutesSinceMidnight % 60;
	}

	FString ToString() const {
		return FString::Printf(TEXT("D%03d %02d:%02d"), Day, Hour, Minute);
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameMinutePassed, const FGameTimeSystem&, NewTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameHourPassed, const FGameTimeSystem&, NewTime);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameMinuteNative, const FGameTimeSystem&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameHourNative, const FGameTimeSystem&);

UCLASS()
class THELONGNIGHT_UNREAL_API UGameTimeSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameTime")
	float TimescaleMinutesPerSecond = 6.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SimTime")
	bool bPaused = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SimTime")
	float MaxDeltaSeconds = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SimTime")
	int32 MaxMinutesPerTick = 240;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="SimTime")
	FGameTimeSystem Current;

	UPROPERTY(BlueprintAssignable, Category="SimTime")
	FOnGameMinutePassed OnGameMinute;

	UPROPERTY(BlueprintAssignable, Category="SimTime")
	FOnGameHourPassed OnGameHour;

	FOnGameMinuteNative OnGameMinuteNative;
	FOnGameHourNative OnGameHourNative;

	UFUNCTION(BlueprintCallable, Category="SimTime")
	void ResetTime(int32 Day = 1, int32 Hour = 8, int32 Minute = 0);

	UFUNCTION(BlueprintCallable, Category="SimTime")
	void AdvanceByRealSeconds(float RealSeconds);

	virtual void Tick(float DeltaTime) override;

	virtual ETickableTickType GetTickableTickType() const override {
		return ETickableTickType::Always;
	}

	virtual TStatId GetStatId() const override {
		RETURN_QUICK_DECLARE_CYCLE_STAT(UGameTime, STATGROUP_Tickables);
	}

private:
	double AccumulatedMinutes = 0.0;
};
