#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PickableItemManager.generated.h"

struct FPickableItemRow;

UCLASS(BlueprintType)
class THELONGNIGHT_UNREAL_API UPickableItemManager : public UObject
{
	GENERATED_BODY()

public:
	// В редакторе задаём ссылку на DataTable asset с RowStruct = PickableItemRow
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Data")
	UDataTable* ItemTable;

	// Получает строку по RowName (RowName может быть, например, "Apple", "Soda")
	UFUNCTION(BlueprintCallable, Category="Items")
	bool GetItemDataByRowName(FName RowName, FPickableItemRow& OutRow) const;
};
