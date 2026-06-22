#include "Items/SPickupItem.h"

#include "Character/SCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Items/SItemData.h"

ASPickupItem::ASPickupItem()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);

	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Block);
}

void ASPickupItem::Interact(AActor* Interactor)
{
	ASCharacter* Character = Cast<ASCharacter>(Interactor);
	if (!IsValid(Character))
	{
		UE_LOG(LogTemp, Warning, TEXT("Pickup failed: interactor is not ASCharacter."));
		return;
	}

	if (!IsValid(ItemData))
	{
		UE_LOG(LogTemp, Warning, TEXT("Pickup failed: ItemData is not assigned on %s."), *GetName());
		return;
	}

	const bool bAdded = Character->AddItemToInventory(ItemData, Quantity);
	if (!bAdded)
	{
		return;
	}

	Destroy();
}

FText ASPickupItem::GetInteractionText() const
{
	if (!IsValid(ItemData))
	{
		return FText::FromString(TEXT("Pick up"));
	}

	if (Quantity <= 1)
	{
		return FText::Format(
			FText::FromString(TEXT("Pick up {0}")),
			ItemData->GetDisplayName()
		);
	}

	return FText::Format(
		FText::FromString(TEXT("Pick up {0} x{1}")),
		ItemData->GetDisplayName(),
		FText::AsNumber(Quantity)
	);
}
