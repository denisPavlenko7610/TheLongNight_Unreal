#include "Interaction/SSimpleInteractableActor.h"

#include "Components/StaticMeshComponent.h"

ASSimpleInteractableActor::ASSimpleInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);

	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Block);
}

void ASSimpleInteractableActor::Interact(AActor* Interactor)
{
	if (!bCanInteract)
	{
		UE_LOG(LogTemp, Log, TEXT("%s cannot be interacted with right now."), *GetName());
		return;
	}

	const FString InteractorName = IsValid(Interactor) ? Interactor->GetName() : TEXT("Unknown");

	UE_LOG(
		LogTemp,
		Log,
		TEXT("%s interacted with %s. Text: %s"),
		*InteractorName,
		*GetName(),
		*InteractionText.ToString()
	);
}

FText ASSimpleInteractableActor::GetInteractionText() const
{
	return InteractionText;
}
