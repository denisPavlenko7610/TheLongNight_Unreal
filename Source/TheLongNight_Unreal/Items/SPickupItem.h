#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SInteractable.h"
#include "SPickupItem.generated.h"

class UStaticMeshComponent;
class USItemData;

UCLASS()
class THELONGNIGHT_UNREAL_API ASPickupItem : public AActor, public ISInteractable
{
	GENERATED_BODY()

public:
	ASPickupItem();

	virtual void Interact(AActor* Interactor) override;
	virtual FText GetInteractionText() const override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Survival|Item")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(EditAnywhere, Category = "Survival|Item")
	TObjectPtr<USItemData> ItemData;

	UPROPERTY(EditAnywhere, Category = "Survival|Item", meta = (ClampMin = "1"))
	int32 Quantity = 1;
};
