#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SInteractable.h"
#include "SSimpleInteractableActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class THELONGNIGHT_UNREAL_API ASSimpleInteractableActor : public AActor, public ISInteractable
{
	GENERATED_BODY()

public:
	ASSimpleInteractableActor();

	virtual void Interact(AActor* Interactor) override;
	virtual FText GetInteractionText() const override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Survival|Interaction")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(EditAnywhere, Category = "Survival|Interaction")
	FText InteractionText = FText::FromString(TEXT("Interact"));

	UPROPERTY(EditAnywhere, Category = "Survival|Interaction")
	bool bCanInteract = true;
};
