#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Save/SSaveTypes.h"
#include "SGameSaveSubsystem.generated.h"

class USGameSaveObject;

UCLASS()
class THELONGNIGHT_UNREAL_API USGameSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	bool SaveCurrentGame(const FString& SlotName);
	bool LoadCurrentGame(const FString& SlotName);

private:
	bool BuildSaveData(FSGameSaveData& OutSaveData) const;
	bool ApplySaveData(const FSGameSaveData& SaveData) const;

	USGameSaveObject* CreateSaveObject(const FString& SlotName, const FSGameSaveData& SaveData) const;
};
