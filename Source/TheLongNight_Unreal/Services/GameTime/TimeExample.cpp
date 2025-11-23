// #include "ASomeActorExample.h"
// #include "GameTimeSubsystem.h"
//
// void ASomeActorExample::BeginPlay()
// {
// 	Super::BeginPlay();
// 	if (UWorld* W = GetWorld())
// 	{
// 		if (auto* TimeSys = W->GetSubsystem<UGameTimeSubsystem>())
// 		{
// 			// Подписка на события (C++):
// 			TimeSys->OnGameMinute.AddDynamic(this, &ASomeActorExample::OnMinute);
//
// 			// Пример: ускорить время (12 мин/сек)
// 			TimeSys->TimescaleMinutesPerSecond = 12.f;
// 		}
// 	}
// }
//
// void ASomeActorExample::OnMinute(const FGameTime& NewTime)
// {
// 	// Обновить статы/погоду и т.д.
// }
