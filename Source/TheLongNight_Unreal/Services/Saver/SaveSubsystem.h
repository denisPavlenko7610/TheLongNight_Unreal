#pragma once

#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Saver.h"
#include "SaveSubsystem.generated.h"

UCLASS()
class THELONGNIGHT_UNREAL_API USaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Save") FString SlotName = TEXT("Survivor01");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Save") int32 UserIndex = 0;

	UFUNCTION(BlueprintCallable, Category="Save") bool SaveNow(USaver* Snapshot);
	UFUNCTION(BlueprintCallable, Category="Save") USaver* LoadNow(bool& bOk);
	UFUNCTION(BlueprintCallable, Category="Save") void DeleteSave();
	UFUNCTION(BlueprintCallable, Category="Save") bool Exists() const;

private:
	void MigrateIfNeeded(USaver* Save);
};
