
#include "PepCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Pepccine/Character/Controller/PepccinePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

APepCharacter::APepCharacter()
{
  PrimaryActorTick.bCanEverTick = true;

  SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
  SpringArmComp->SetupAttachment(RootComponent);
  SpringArmComp->TargetArmLength = CameraArmLength;
  SpringArmComp->bUsePawnControlRotation = true;

  CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
  CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
  CameraComp->bUsePawnControlRotation = false;

  PlayerStatComponent = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("PlayerStatComponent"));
  
}

void APepCharacter::BeginPlay()
{
  Super::BeginPlay();

  DefineCharacterMovement();
}

void APepCharacter::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

}

void APepCharacter::DefineCharacterMovement()
{
  if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
  {
    MovementComponent->GetNavAgentPropertiesRef().bCanCrouch = true;
  }
}

void APepCharacter::Move(const FInputActionValue& Value)
{
  FVector2D MoveInput = Value.Get<FVector2D>();

  UE_LOG(LogTemp, Log, TEXT("MovementVector: [%s]"), *MoveInput.ToString());

  if (!FMath::IsNearlyZero(MoveInput.X))
  {
    AddMovementInput(GetActorForwardVector(), MoveInput.X);
  }

  if (!FMath::IsNearlyZero(MoveInput.Y))
  {
    AddMovementInput(GetActorRightVector(), MoveInput.Y);
  }
}

void APepCharacter::JumpStart()
{
  Super::Jump();

  UE_LOG(LogTemp, Log, TEXT("JumpStart!"));

  Jump();
}

void APepCharacter::JumpStop()
{
  Super::StopJumping();

  StopJumping();
  UE_LOG(LogTemp, Log, TEXT("JumpStop!"));
}

void APepCharacter::UseItem()
{
  UE_LOG(LogTemp, Log, TEXT("UseItem!"));
}

void APepCharacter::Look(const FInputActionValue& value)
{
  FVector2D LookInput = value.Get<FVector2D>();

  UE_LOG(LogTemp, Log, TEXT("LookInput[%s]"), *LookInput.ToString());

  AddControllerYawInput(LookInput.X);
  AddControllerPitchInput(LookInput.Y);
}

void APepCharacter::StartSprint(const FInputActionValue& value)
{
  UE_LOG(LogTemp, Log, TEXT("StartSprint!"));
  if (GetCharacterMovement())
  {
    GetCharacterMovement()->MaxWalkSpeed = PlayerStatComponent->SprintSpeed;
  }
}

void APepCharacter::StopSprint(const FInputActionValue& value)
{
  UE_LOG(LogTemp, Log, TEXT("StopSprint!"));
  if (GetCharacterMovement())
  {
    GetCharacterMovement()->MaxWalkSpeed = PlayerStatComponent->MovementSpeed;
  }
}

void APepCharacter::Crouching()
{
  if (!GetCharacterMovement())
    return;

  bIsCrouching = GetCharacterMovement()->IsCrouching();

  UE_LOG(LogTemp, Log, TEXT("Crouching State Before: [%d]"), bIsCrouching);

  if (bIsCrouching)
  {
    UnCrouch();
  }
  else
  {
    Crouch();
  }

  UE_LOG(LogTemp, Log, TEXT("Crouching State After: [%d]"), GetCharacterMovement()->IsCrouching());
}

void APepCharacter::Reload()
{
  UE_LOG(LogTemp, Log, TEXT("Reload!"));


}

void APepCharacter::Interactive()
{
  UE_LOG(LogTemp, Log, TEXT("Interactive!"));


}

void APepCharacter::OpenInventory()
{
  UE_LOG(LogTemp, Log, TEXT("OpenInventory!"));

  // HUD
}

void APepCharacter::SwapItem(const FInputActionValue& value)
{
  UE_LOG(LogTemp, Warning, TEXT("SwapItem"));
  float ScrollValue = value.Get<float>();

  if (ScrollValue > 0.0f)
  {
    UE_LOG(LogTemp, Log, TEXT("Swapping Forward [%f]"), ScrollValue);
  }
  else if (ScrollValue < 0.0f)
  {
    UE_LOG(LogTemp, Log, TEXT("Swapping Backward [%f]"), ScrollValue);
  }
}

void APepCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
  if (!EnhancedInput) return;

  APepccinePlayerController* PlayerController = Cast<APepccinePlayerController>(GetController());
  if (!PlayerController) return;

  // MoveAction: W A S D
  if (PlayerController->MoveAction)
  {
    EnhancedInput->BindAction(
      PlayerController->MoveAction,
      ETriggerEvent::Triggered,
      this,
      &APepCharacter::Move
    );
  }

  // LookAction: Mouse2D
  if (PlayerController->LookAction)
  {
    EnhancedInput->BindAction(
      PlayerController->LookAction,
      ETriggerEvent::Triggered,
      this,
      &APepCharacter::Look
    );
  }

  // Sprint: Shift
  if (PlayerController->SprintAction)
  {
    EnhancedInput->BindAction(
      PlayerController->SprintAction,
      ETriggerEvent::Triggered,
      this,
      &APepCharacter::StartSprint
    );
  }

  if (PlayerController->SprintAction)
  {
    EnhancedInput->BindAction(
      PlayerController->SprintAction,
      ETriggerEvent::Completed,
      this,
      &APepCharacter::StopSprint
    );
  }

  // Jump: Space
  if (PlayerController->JumpAction)
  {
    EnhancedInput->BindAction(
      PlayerController->JumpAction,
      ETriggerEvent::Triggered,
      this,
      &APepCharacter::JumpStart
    );
  }

  if (PlayerController->JumpAction)
  {
    EnhancedInput->BindAction(
      PlayerController->JumpAction,
      ETriggerEvent::Completed,
      this,
      &APepCharacter::JumpStop
    );
  }

  // UseItem: Q
  if (PlayerController->ItemUseAction)
  {
    EnhancedInput->BindAction(
      PlayerController->ItemUseAction,
      ETriggerEvent::Triggered,
      this,
      &APepCharacter::UseItem
    );
  }

  // Crouch: Ctrl
  if (PlayerController->CrouchAction)
  {
    EnhancedInput->BindAction(
      PlayerController->CrouchAction,
      ETriggerEvent::Started,
      this,
      &APepCharacter::Crouching
    );
  }

  // Reload: R
  if (PlayerController->ReloadingAction)
  {
    EnhancedInput->BindAction(
      PlayerController->ReloadingAction,
      ETriggerEvent::Triggered,
      this,
      &APepCharacter::Reload
    );
  }

  // Interactive: E
  if (PlayerController->InteractiveAction)
  {
    EnhancedInput->BindAction(
      PlayerController->InteractiveAction,
      ETriggerEvent::Triggered,
      this,
      &APepCharacter::Interactive
    );
  }

  // Inventory: Tab
  if (PlayerController->InventoryAction)
  {
    EnhancedInput->BindAction(
      PlayerController->InventoryAction,
      ETriggerEvent::Triggered,
      this,
      &APepCharacter::OpenInventory
    );
  }

  // Swap: Mouse Wheel
  if (PlayerController->SwapAction)
  {
    EnhancedInput->BindAction(
      PlayerController->SwapAction,
      ETriggerEvent::Triggered,
      this,
      &APepCharacter::SwapItem
    );
  }
}