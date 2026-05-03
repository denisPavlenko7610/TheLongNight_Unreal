// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS(abstract)
class THELONGNIGHT_UNREAL_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASPlayerController();

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* PauseAction;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input", meta = (AllowPrivateAccess = "true"))
	TArray<UInputMappingContext*> PlayerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input", meta = (AllowPrivateAccess = "true"))
	TArray<UInputMappingContext*> GameStateMappingContext;

	virtual void SetupInputComponent() override;

private:
	void OnPausePressed();
};
