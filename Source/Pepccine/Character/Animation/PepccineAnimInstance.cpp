#include "Character/Animation/PepccineAnimInstance.h"
#include "Character/Player/PepCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UPepccineAnimInstance::UPepccineAnimInstance()
{
	MovingThreshould = 3.0f;
	Speed = 0.0f;
	Direction = 0.0f;
	WalkSpeed = 0.0f;
	Velocity = FVector::ZeroVector;
	bIsIdle = true;
	bIsCrouch = false;
	bIsSprint = false;
	bIsFalling = false;
	bIsMainWeapon = false;
	ControllerPitch = 0.0f;
}

void UPepccineAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<APepCharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UPepccineAnimInstance::NativeUpdateAnimation(float dt)
{
	Super::NativeUpdateAnimation(dt);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		Speed = Velocity.Size2D();
		Direction = CalculateDirection(Velocity, Owner->GetActorRotation());
		bIsIdle = Speed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsCrouch = Movement->IsCrouching();
		bIsSprint = Owner->bIsSprinting;
		float Pitch = Owner->GetControlRotation().Pitch;
		(Pitch > 90) ? ControllerPitch = Pitch - 360 : ControllerPitch = Pitch;
		bIsMainWeapon = Owner->bIsMainWeaponEquipped;
	}
}

void UPepccineAnimInstance::AnimNotify_EndReloading()
{
	UE_LOG(LogTemp, Log, TEXT("EndReload!"));
	Owner->bIsReloading = false;
}