#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USInventoryComponent;
class USItemData;
class UCameraComponent;

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

	USInventoryComponent* GetInventoryComponent() const;

private:
	bool FindInteractionHit(FHitResult& OutHitResult) const;

private:
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
};
