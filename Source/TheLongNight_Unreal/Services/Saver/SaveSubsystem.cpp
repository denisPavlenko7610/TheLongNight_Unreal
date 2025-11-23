#include "SaveSubsystem.h"

#include "SaveSubsystem.h"
#include "Kismet/GameplayStatics.h"

bool USaveSubsystem::SaveNow(USaver* Snapshot)
{
	if (!Snapshot) {
		return false;
	}
	Snapshot->SaveUtcIso = FDateTime::UtcNow().ToIso8601();
	return UGameplayStatics::SaveGameToSlot(Snapshot, SlotName, UserIndex);
}

USaver* USaveSubsystem::LoadNow(bool& bOk)
{
	bOk = false;
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex)) {
		return nullptr;
	}

	TCopyQualifiersFromTo_T<USaveGame, USaver>* Loaded = Cast<USaver>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
	if (!Loaded) {
		return nullptr;
	}

	MigrateIfNeeded(Loaded);
	bOk = true;
	return Loaded;
}

void USaveSubsystem::DeleteSave()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex)) {
		UGameplayStatics::DeleteGameInSlot(SlotName, UserIndex);
	}
}

bool USaveSubsystem::Exists() const
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex);
}

void USaveSubsystem::MigrateIfNeeded(USaver* Save)
{
	if (!Save) {
		return;
	}

	if (Save->Version < 1)
	{
		Save->Inventory.CarryCapacityGrams = FMath::Max(1, Save->Inventory.CarryCapacityGrams);
		Save->Version = 1;
	}
}
