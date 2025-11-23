#include "Misc/AutomationTest.h"
#include "Services/GameTime/GameTimeSystem.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FGameTime_AddMinutes_CrossesToNextDay,
	"Project.GameTime.AddMinutes_CrossesToNextDay",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter
)

bool FGameTime_AddMinutes_CrossesToNextDay::RunTest(const FString& Parameters)
{
	// Arrange
	FGameTimeSystem Time;
	Time.Day = 1;
	Time.Hour = 23;
	Time.Minute = 50;

	// Act
	Time.AddMinutes(20);

	// Assert
	TestEqual(TEXT("Day advanced to next day"), Time.Day, 2);
	TestEqual(TEXT("Hour wrapped to 0"),        Time.Hour, 0);
	TestEqual(TEXT("Minute is 10"),             Time.Minute, 10);

	return true;
}
