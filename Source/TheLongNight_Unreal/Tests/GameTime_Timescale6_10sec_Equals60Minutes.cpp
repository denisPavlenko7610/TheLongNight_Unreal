#include "Misc/AutomationTest.h"
#include "Services/GameTime/GameTimeSystem.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FGameTime_Timescale6_10sec_Equals60Minutes,
	"Project.GameTime.Timescale6_10sec_Equals60Minutes",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool FGameTime_Timescale6_10sec_Equals60Minutes::RunTest(const FString& Parameters)
{
	// Arrange
	FGameTimeSystem Time; // no UWorld, no subsystem
	Time.Day = 1; Time.Hour = 0; Time.Minute = 0;

	const float TimescaleMinutesPerSecond = 6.f;
	const float MaxDeltaSeconds = 1.f;
	const int32 MaxMinutesPerTick = 1000;

	int32 MinutesPassed = 0;
	int32 HoursPassed   = 0;
	float AccumulatedMinutes = 0.f;

	// Act: simulate 10 seconds
	for (int i = 0; i < 10; ++i)
	{
		const float Step = FMath::Min(1.f, MaxDeltaSeconds);
		AccumulatedMinutes += Step * TimescaleMinutesPerSecond;

		int32 Ready = FMath::FloorToInt(AccumulatedMinutes);
		int32 ToProcess = FMath::Min(Ready, MaxMinutesPerTick);
		AccumulatedMinutes -= ToProcess;

		for (int32 m = 0; m < ToProcess; ++m)
		{
			const int32 PrevHour = Time.Hour;
			Time.AddMinutes(1);
			++MinutesPassed;
			if (Time.Hour != PrevHour) { ++HoursPassed; }
		}
	}

	// Assert
	TestEqual(TEXT("Minutes passed"), MinutesPassed, 60);
	TestEqual(TEXT("Hours passed"),   HoursPassed,   1);
	TestEqual(TEXT("Current hour"),   Time.Hour,     1);
	TestEqual(TEXT("Current minute"), Time.Minute,   0);

	return true;
}
