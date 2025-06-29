
#include "SurvivePlayer.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "PlayerInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ASurvivePlayer::ASurvivePlayer()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));
	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));
	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	PlayerInteractionComponent = CreateDefaultSubobject<UPlayerInteractionComponent>(TEXT("Player interaction component"));
	
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
}

void ASurvivePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	if (UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		input->BindAction(JumpAction, ETriggerEvent::Started, this, &ASurvivePlayer::DoJumpStart);
		input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASurvivePlayer::DoJumpEnd);

		input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASurvivePlayer::MoveInput);
		input->BindAction(SprintAction, ETriggerEvent::Started, this, &ASurvivePlayer::StartSprinting);
		input->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASurvivePlayer::StopSprinting);

		input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASurvivePlayer::LookInput);
		input->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ASurvivePlayer::LookInput);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASurvivePlayer::MoveInput(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
}

void ASurvivePlayer::LookInput(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	DoAim(LookAxisVector.X, LookAxisVector.Y);
}

void ASurvivePlayer::StartSprinting()
{
	_isSprinting = true;
	UpdateMovementSpeed();
}

void ASurvivePlayer::StopSprinting()
{
	_isSprinting = false;
	UpdateMovementSpeed();
}

void ASurvivePlayer::UpdateMovementSpeed()
{
	float TargetSpeed = _isSprinting ? _sprintSpeed : _walkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = TargetSpeed;
}

void ASurvivePlayer::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ASurvivePlayer::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void ASurvivePlayer::DoJumpStart()
{
	Jump();
}

void ASurvivePlayer::DoJumpEnd()
{
	StopJumping();
}
