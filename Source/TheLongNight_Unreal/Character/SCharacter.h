#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USInventoryComponent;
class USItemData;
class UCameraComponent;
class USCharacterSurvivalComponent;

UCLASS()
class THELONGNIGHT_UNREAL_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASCharacter();

	virtual void BeginPlay() override;

	void Move(const FVector2D& MoveVector);
	void Look(const FVector2D& LookVector);

	void StartSprint();
	void StopSprint();

	void TryInteract();
	bool GetFocusedInteractionText(FText& OutInteractionText) const;

	bool AddItemToInventory(USItemData* ItemData, int32 Quantity = 1);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	USCharacterSurvivalComponent* GetSurvivalComponent() const;
	USInventoryComponent* GetInventoryComponent() const;

private:
	bool FindInteractionHit(FHitResult& OutHitResult) const;

	void BindToGameTime();
	void UnbindFromGameTime();
	void HandleGameMinutePassed(float GameMinutes);
	
	UPROPERTY(VisibleAnywhere, Category = "Survival|Camera")
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Movement")
	float WalkSpeed = 280.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Movement")
	float SprintSpeed = 520.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Look")
	float LookSensitivity = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Interaction")
	float InteractionDistance = 250.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Survival|Interaction")
	TEnumAsByte<ECollisionChannel> InteractionTraceChannel = ECC_Visibility;

	UPROPERTY(VisibleAnywhere, Category = "Survival|Inventory")
	TObjectPtr<USInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, Category = "Survival|Vitals")
	TObjectPtr<USCharacterSurvivalComponent> SurvivalComponent;
};
