// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SurvivePlayerController.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS(abstract)
class THELONGNIGHT_UNREAL_API ASurvivePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASurvivePlayerController();

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
