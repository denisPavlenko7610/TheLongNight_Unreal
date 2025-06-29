#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractionComponent.generated.h"

class APickableItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHoverSignature, APickableItem*, Item);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THELONGNIGHT_UNREAL_API UPlayerInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Interaction")
	float MaxDistance = 300.f;

	UPROPERTY(EditAnywhere, Category="Interaction")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_GameTraceChannel2;

	UPROPERTY(BlueprintAssignable)
	FOnHoverSignature OnHoverEnter;

	UPROPERTY(BlueprintAssignable)
	FOnHoverSignature OnHoverExit;

protected:
	UPlayerInteractionComponent();
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float Delta, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	APickableItem* CurrentItem = nullptr;

	void HandleHover();
	void HandlePickup();
};
