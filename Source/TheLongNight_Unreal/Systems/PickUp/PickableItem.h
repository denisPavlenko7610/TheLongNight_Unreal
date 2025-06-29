#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "PickableItem.generated.h"

struct FPickableItemRow;

UCLASS()
class THELONGNIGHT_UNREAL_API APickableItem : public AActor
{
	GENERATED_BODY()

public:
	APickableItem();

	UPROPERTY(EditAnywhere, Category="Item")
	FDataTableRowHandle ItemRowHandle;

	UPROPERTY(VisibleAnywhere, Category="Item")
	UStaticMeshComponent* MeshComp;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void PickUp();

	UFUNCTION(BlueprintCallable, Category="Item")
	FPickableItemRow GetDataRow() const;
};
