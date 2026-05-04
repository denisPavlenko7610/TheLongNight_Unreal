#include "Character/SCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interaction/SInteractable.h"
#include "Inventory/SInventoryComponent.h"
#include "Items/SItemData.h"

ASCharacter::ASCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

    FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
    FirstPersonCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f));
    FirstPersonCamera->bUsePawnControlRotation = true;

    bUseControllerRotationYaw = true;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

    UCharacterMovementComponent* Movement = GetCharacterMovement();
    Movement->MaxWalkSpeed = WalkSpeed;
    Movement->bOrientRotationToMovement = false;
    Movement->bUseControllerDesiredRotation = false;
    Movement->JumpZVelocity = 0.0f;
    Movement->AirControl = 0.0f;

    InventoryComponent = CreateDefaultSubobject<USInventoryComponent>(TEXT("InventoryComponent"));
}

void ASCharacter::BeginPlay()
{
    Super::BeginPlay();

    UCharacterMovementComponent* Movement = GetCharacterMovement();
    if (IsValid(Movement))
    {
        Movement->MaxWalkSpeed = WalkSpeed;
    }
}

void ASCharacter::Move(const FVector2D& MoveVector)
{
    if (!Controller)
    {
        UE_LOG(LogTemp, Warning, TEXT("Move failed: Character has no Controller."));
        return;
    }

    if (MoveVector.IsNearlyZero())
    {
        return;
    }

    const FRotator ControlRotation = Controller->GetControlRotation();
    const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    AddMovementInput(ForwardDirection, MoveVector.Y);
    AddMovementInput(RightDirection, MoveVector.X);
}

void ASCharacter::Look(const FVector2D& LookVector)
{
    if (LookVector.IsNearlyZero())
    {
        return;
    }

    AddControllerYawInput(LookVector.X * LookSensitivity);
    AddControllerPitchInput(LookVector.Y * LookSensitivity);
}

void ASCharacter::StartSprint()
{
    UCharacterMovementComponent* Movement = GetCharacterMovement();
    if (!IsValid(Movement))
    {
        return;
    }

    Movement->MaxWalkSpeed = SprintSpeed;
}

void ASCharacter::StopSprint()
{
    UCharacterMovementComponent* Movement = GetCharacterMovement();
    if (!IsValid(Movement))
    {
        return;
    }

    Movement->MaxWalkSpeed = WalkSpeed;
}

void ASCharacter::TryInteract()
{
    FHitResult HitResult;
    if (!FindInteractionHit(HitResult))
    {
        UE_LOG(LogTemp, Verbose, TEXT("Interaction failed: no hit."));
        return;
    }

    AActor* HitActor = HitResult.GetActor();
    if (!IsValid(HitActor))
    {
        UE_LOG(LogTemp, Verbose, TEXT("Interaction failed: hit actor is invalid."));
        return;
    }

    ISInteractable* Interactable = Cast<ISInteractable>(HitActor);
    if (!Interactable)
    {
        UE_LOG(LogTemp, Verbose, TEXT("Interaction failed: %s is not interactable."), *HitActor->GetName());
        return;
    }

    Interactable->Interact(this);
}

bool ASCharacter::FindInteractionHit(FHitResult& OutHitResult) const
{
    if (!IsValid(FirstPersonCamera))
    {
        return false;
    }

    const UWorld* World = GetWorld();
    if (!IsValid(World))
    {
        return false;
    }

    const FVector Start = FirstPersonCamera->GetComponentLocation();
    const FVector End = Start + FirstPersonCamera->GetForwardVector() * InteractionDistance;

    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(SInteractionTrace), false);
    QueryParams.AddIgnoredActor(this);

    return World->LineTraceSingleByChannel(
        OutHitResult,
        Start,
        End,
        InteractionTraceChannel,
        QueryParams
    );
}

bool ASCharacter::GetFocusedInteractionText(FText& OutInteractionText) const {
    FHitResult HitResult;
    if (!FindInteractionHit(HitResult))
    {
        return false;
    }

    AActor* HitActor = HitResult.GetActor();
    if (!IsValid(HitActor))
    {
        return false;
    }

    ISInteractable* Interactable = Cast<ISInteractable>(HitActor);
    if (!Interactable)
    {
        return false;
    }

    OutInteractionText = Interactable->GetInteractionText();
    return true;
}

bool ASCharacter::AddItemToInventory(USItemData* ItemData, int32 Quantity)
{
    if (!IsValid(InventoryComponent))
    {
        UE_LOG(LogTemp, Warning, TEXT("AddItemToInventory failed: InventoryComponent is invalid."));
        return false;
    }

    return InventoryComponent->AddItem(ItemData, Quantity);
}

USInventoryComponent* ASCharacter::GetInventoryComponent() const
{
    return InventoryComponent;
}
