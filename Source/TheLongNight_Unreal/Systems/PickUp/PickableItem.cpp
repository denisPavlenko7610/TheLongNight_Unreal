#include "PickableItem.h"
#include "Components/StaticMeshComponent.h"
#include "Data/PickableItemRow.h"

APickableItem::APickableItem()
{
	PrimaryActorTick.bCanEverTick = false;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;
}

void APickableItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (ItemRowHandle.DataTable)
	{
		const FString Context(TEXT("APickableItem_OnConstruction"));
		const FPickableItemRow* Row = ItemRowHandle.DataTable
		                                           ->FindRow<FPickableItemRow>(ItemRowHandle.RowName, Context);

		if (Row && Row->PreviewMesh)
		{
			MeshComp->SetStaticMesh(Row->PreviewMesh);
			MeshComp->SetRelativeRotation(Row->PreviewMeshRotation);
		}
		else
		{
			MeshComp->SetStaticMesh(nullptr);
		}

#if WITH_EDITOR
		if (Row && !Row->ItemName.IsEmpty())
		{
			SetActorLabel(Row->ItemName.ToString());
		}
		else
		{
			SetActorLabel(TEXT("PickableItem"));
		}
#endif
	}
	else
	{
		MeshComp->SetStaticMesh(nullptr);
	}
}

void APickableItem::PickUp()
{
	Destroy();
}

FPickableItemRow APickableItem::GetDataRow() const
{
	FPickableItemRow OutRow;
	if (ItemRowHandle.DataTable)
	{
		const FString Context(TEXT("GetDataRow"));
		if (const FPickableItemRow* Found = ItemRowHandle.DataTable->FindRow<FPickableItemRow>(
			ItemRowHandle.RowName, Context))
		{
			OutRow = *Found;
		}
	}
	return OutRow;
}
