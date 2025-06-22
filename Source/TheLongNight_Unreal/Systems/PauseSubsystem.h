#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "PauseSubsystem.generated.h"

UCLASS(BlueprintType)
class THELONGNIGHT_UNREAL_API UPauseSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void TogglePause();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Pause")
	TSubclassOf<UUserWidget> PauseMenuClass;
	
	UPROPERTY()
	UUserWidget* PauseMenuInstance;

	APlayerController* GetFirstPlayerController() const;
    
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
