#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Save/SSaveTypes.h"
#include "SGameSaveObject.generated.h"

UCLASS()
class THELONGNIGHT_UNREAL_API USGameSaveObject : public USaveGame
{
	GENERATED_BODY()

public:
	const FString& GetSlotName() const
	{
		return SlotName;
	}

	void SetSlotName(const FString& NewSlotName)
	{
		SlotName = NewSlotName;
	}

	const FSGameSaveData& GetSaveData() const
	{
		return SaveData;
	}

	FSGameSaveData& GetMutableSaveData()
	{
		return SaveData;
	}

private:
	UPROPERTY()
	FString SlotName = TEXT("Slot_01");

	UPROPERTY()
	FSGameSaveData SaveData;
};
