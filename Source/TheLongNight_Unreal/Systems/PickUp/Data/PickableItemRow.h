#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"      
#include "ItemType.h"           
#include "PickableItemRow.generated.h"

USTRUCT(BlueprintType)
struct THELONGNIGHT_UNREAL_API FPickableItemRow : public FTableRowBase
{
	GENERATED_BODY()
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	EItemType ItemType = EItemType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stats")
	float Calories = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stats")
	float Weight = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item", meta=(MultiLine=true))
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Preview")
	UStaticMesh* PreviewMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Preview")
	FRotator PreviewMeshRotation = FRotator();
};
