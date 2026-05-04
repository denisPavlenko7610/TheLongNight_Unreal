#include "Character/SCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interaction/SInteractable.h"

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
