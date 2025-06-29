#include "PlayerInteractionComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Systems/PickUp/PickableItem.h"

UPlayerInteractionComponent::UPlayerInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(true);
}

void UPlayerInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	HandleHover();
	HandlePickup();
}

void UPlayerInteractionComponent::HandleHover()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
	if (!PlayerController)
		return;

	FVector StartPosition;
	FRotator StartRotation;
	PlayerController->GetPlayerViewPoint(StartPosition, StartRotation);

	FVector EndPosition = StartPosition + StartRotation.Vector() * MaxDistance;
	FHitResult Hit;
	FCollisionQueryParams params(NAME_None, false, GetOwner());

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartPosition, EndPosition, TraceChannel, params);

	APickableItem* HitItem = bHit ? Cast<APickableItem>(Hit.GetActor()) : nullptr;

	if (HitItem != CurrentItem)
	{
		if (CurrentItem)
			OnHoverExit.Broadcast(CurrentItem);
		
		CurrentItem = HitItem;
		
		if (CurrentItem)
			OnHoverEnter.Broadcast(CurrentItem);
	}

	//DrawDebugLine(GetWorld(), StartPosition, EndPosition, FColor::Red, false, -1, 0, 1.f);
}

void UPlayerInteractionComponent::HandlePickup()
{
	if (CurrentItem && GetWorld()->GetFirstPlayerController() && GetWorld()->GetFirstPlayerController()->IsInputKeyDown(
		EKeys::LeftMouseButton))
	{
		CurrentItem->PickUp();
		CurrentItem = nullptr;
	}
}
