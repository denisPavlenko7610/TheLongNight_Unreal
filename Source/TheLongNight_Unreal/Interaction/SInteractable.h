#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SInteractable.generated.h"

UINTERFACE()
class THELONGNIGHT_UNREAL_API USInteractable : public UInterface
{
	GENERATED_BODY()
};

class THELONGNIGHT_UNREAL_API ISInteractable
{
	GENERATED_BODY()

public:
	virtual void Interact(AActor* Interactor) = 0;

	virtual FText GetInteractionText() const
	{
		return FText::FromString(TEXT("Interact"));
	}
};
