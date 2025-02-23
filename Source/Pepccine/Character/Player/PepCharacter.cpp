
#include "PepCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "CrosshairHUDComponent.h"
#include "PrograssBarHUDComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Character/Animation/PepccineMontageComponent.h"

APepCharacter::APepCharacter()
{
  PrimaryActorTick.bCanEverTick = true;

  SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
  SpringArmComp->SetupAttachment(RootComponent);
  SpringArmComp->TargetArmLength = CameraArmLength;
  SpringArmComp->bUsePawnControlRotation = true;

  ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
  ThirdPersonCamera->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
  ThirdPersonCamera->bUsePawnControlRotation = false;

  FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
  FirstPersonCamera->SetupAttachment(RootComponent);
  FirstPersonCamera->bUsePawnControlRotation = true;

  PlayerStatComponent = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("PlayerStatComponent"));

  CrosshairComponent = CreateDefaultSubobject<UCrosshairHUDComponent>(TEXT("CrosshairHUDComponent"));
  PrograssBarComponent = CreateDefaultSubobject<UPrograssBarHUDComponent>(TEXT("PrograssBarComponent"));

  PepccineMontageComponent = CreateDefaultSubobject<UPepccineMontageComponent>(TEXT("MontageComponent"));
}

void APepCharacter::BeginPlay()
{
  Super::BeginPlay();

  InitializeCharacterMovement();
  AddObservers();
}

// Initialize Character Status
#pragma region
void APepCharacter::InitializeCharacterMovement()
{
  if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
  {
    MovementComponent->GetNavAgentPropertiesRef().bCanCrouch = true;
    MovementComponent->MaxWalkSpeed = PlayerStatComponent->MovementSpeed;
    MovementComponent->JumpZVelocity = PlayerStatComponent->JumpZVelocity;
  }
}
#pragma endregion

void APepCharacter::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  CheckSprinting();
}

// Tick Method
#pragma region
void APepCharacter::CheckSprinting()
{
  if (bIsSprinting) {
    if (!PlayerStatComponent->DecreaseStamina(0.25))
    {
      bIsSprinting = false;
      return;
    }
    SetCharacterSpeed(PlayerStatComponent->SprintSpeed);
  }
  else {
    SetCharacterSpeed(PlayerStatComponent->MovementSpeed);
  }
}
#pragma endregion

// Observers
#pragma region
void APepCharacter::AddObservers()
{
  PlayerStatComponent->AddStaminaObserver(this);
}

void APepCharacter::OnStaminaChanged(float NewStamina, float MaxStamina)
{
  if (!PrograssBarComponent) return;
  //UE_LOG(LogTemp, Warning, TEXT("Stamina Updated: %f / %f"), NewStamina, MaxStamina);
  PrograssBarComponent->SetStamina(NewStamina, MaxStamina);
}
#pragma endregion

// TODO: 분류 세분화 필요
// Action
#pragma region
void APepCharacter::Move(const FInputActionValue& Value)
{
  FVector2D MoveInput = Value.Get<FVector2D>();

  //UE_LOG(LogTemp, Log, TEXT("MovementVector: [%s]"), *MoveInput.ToString());

  bool bWasMoving = bIsMoving;
  bIsMoving = !FMath::IsNearlyZero(MoveInput.X) || !FMath::IsNearlyZero(MoveInput.Y); // 1, 1, 1, 0

  if (bWasMoving && !bIsMoving)
  {
    OnMovementStopped();
  }

  if (!FMath::IsNearlyZero(MoveInput.X))
  {
    AddMovementInput(GetActorForwardVector(), MoveInput.X);
  }

  if (!FMath::IsNearlyZero(MoveInput.Y))
  {
    AddMovementInput(GetActorRightVector(), MoveInput.Y);
  }
}

void APepCharacter::OnMovementStopped()
{
  UE_LOG(LogTemp, Log, TEXT("Movement Stopped!"));
}

void APepCharacter::JumpStart()
{
  Super::Jump();

  UE_LOG(LogTemp, Log, TEXT("JumpStart!"));
  bIsJumping = true;
  Jump();
}

void APepCharacter::JumpStop()
{
  Super::StopJumping();
  bIsJumping = false;
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

  //UE_LOG(LogTemp, Log, TEXT("LookInput[%s]"), *LookInput.ToString());

  AddControllerYawInput(LookInput.X);
  AddControllerPitchInput(LookInput.Y);
}

void APepCharacter::StartSprint(const FInputActionValue& value)
{
  if (bIsRollable)
  {
    SprintHoldStartTime = GetWorld()->GetTimeSeconds();
  }

  if (!bIsSprintable) return;
  bIsSprinting = true;
}

void APepCharacter::StopSprint(const FInputActionValue& value)
{
  float HoldTime = GetWorld()->GetTimeSeconds() - SprintHoldStartTime;
  bIsSprinting = false;

  if (HoldTime <= SprintHoldThreshold)
  {
    Roll();
  }

  SprintHoldStartTime = 0.0f; // 초기화
}

void APepCharacter::SetCharacterSpeed(float Speed)
{
  if (GetCharacterMovement())
  {
    GetCharacterMovement()->MaxWalkSpeed = Speed;
  }
}

void APepCharacter::Roll()
{
  if (!GetCharacterMovement() || bIsRolling || bIsJumping) return;

  bIsRolling = true;

  if (!PlayerStatComponent->DecreaseStaminaByPercentage(30))
  {
    bIsRolling = false;
    return;
  }
  // 점프도중 롤 금지 날라가는 현상
  GetCharacterMovement()->AddImpulse(GetRollDirection(), true);
  GetWorldTimerManager().SetTimer(RollTimerHandle, this, &APepCharacter::EndRoll, 0.1f, false);
}

void APepCharacter::EndRoll()
{
  bIsRolling = false;

  // [임시] 롤 활성시에 해당부분 접근을 못해서 가져옴
  if (GetCharacterMovement())
  {
    GetCharacterMovement()->MaxWalkSpeed = PlayerStatComponent->MovementSpeed;
  }

  UE_LOG(LogTemp, Log, TEXT("Roll Ended!"));
}

FVector APepCharacter::GetRollDirection()
{
  FVector Velocity = GetCharacterMovement()->Velocity;
  FVector RollDirection;

  if (!Velocity.IsNearlyZero())
  {
    RollDirection = Velocity.GetSafeNormal() * PlayerStatComponent->RollingDistance;
  }
  else
  {
    RollDirection = GetActorForwardVector() * PlayerStatComponent->RollingDistance;
  }

  UE_LOG(LogTemp, Log, TEXT("RollDirection! [%s]"), *RollDirection.ToString());

  return RollDirection;
}

void APepCharacter::Crouching()
{
  if (!GetCharacterMovement())
    return;

  bIsCrouching = GetCharacterMovement()->IsCrouching();

  if (bIsCrouching)
  {
    UnCrouch();
    UE_LOG(LogTemp, Log, TEXT("UnCrouch! [%d][%f]"), bIsCrouching, PlayerStatComponent->MovementSpeed);
    GetCharacterMovement()->MaxWalkSpeed = PlayerStatComponent->MovementSpeed;
  }
  else
  {
    Crouch();
    UE_LOG(LogTemp, Log, TEXT("Crouch! [%d][%f]"), bIsCrouching, PlayerStatComponent->CrouchSpeed);
    GetCharacterMovement()->MaxWalkSpeed = PlayerStatComponent->CrouchSpeed;
  }
}

void APepCharacter::Reload()
{
  UE_LOG(LogTemp, Log, TEXT("Reload!"));

  PepccineMontageComponent->Reloading();

  if (bIsReloading)
  {
    
  }
  else
  {

  }
}

void APepCharacter::Interactive()
{
  UE_LOG(LogTemp, Log, TEXT("Interactive!"));

  if (bIsInteracting)
  {

  }
  else
  {

  }
}

void APepCharacter::OpenInventory()
{
  UE_LOG(LogTemp, Log, TEXT("OpenInventory!"));

  // HUD
  if (bIsInventoryOpened)
  {

  }
  else
  {

  }
}

void APepCharacter::SwapItem(const FInputActionValue& value)
{
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

void APepCharacter::Fire()
{
  UE_LOG(LogTemp, Log, TEXT("Fire!"));

  PepccineMontageComponent->Fire();
}

void APepCharacter::ZoomIn()
{
  UE_LOG(LogTemp, Log, TEXT("ZoomIn!"));

  bIsZooming = true;
  
  ToggleCameraView();

  if (!CrosshairComponent) return;
  CrosshairComponent->ShowCrosshair();
}

void APepCharacter::ZoomOut()
{
  UE_LOG(LogTemp, Log, TEXT("ZoomOut!"));

  bIsZooming = false;
  
  ToggleCameraView();

  if (!CrosshairComponent) return;
  CrosshairComponent->HideCrosshair();
}

void APepCharacter::ToggleCameraView()
{
  if (!PlayerController) return;

  bIsFirstPersonView = !bIsFirstPersonView;

  FirstPersonCamera->SetActive(bIsFirstPersonView);
  ThirdPersonCamera->SetActive(!bIsFirstPersonView);
}
#pragma endregion

// Key Mapping
#pragma region
void APepCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
  if (!EnhancedInput) return;

  PlayerController = Cast<APepccinePlayerController>(GetController());
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
      ETriggerEvent::Started,
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
      ETriggerEvent::Started,
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

  // Fire: Mouse Left
  if (PlayerController->FireAction)
  {
    EnhancedInput->BindAction(
      PlayerController->FireAction,
      ETriggerEvent::Started,
      this,
      &APepCharacter::Fire
    );
  }

  // Zoom: Mouse Right
  if (PlayerController->ZoomAction)
  {
    EnhancedInput->BindAction(
      PlayerController->ZoomAction,
      ETriggerEvent::Started,
      this,
      &APepCharacter::ZoomIn
    );
  }

  // Zoom: Mouse Right
  if (PlayerController->ZoomAction)
  {
    EnhancedInput->BindAction(
      PlayerController->ZoomAction,
      ETriggerEvent::Completed,
      this,
      &APepCharacter::ZoomOut
    );
  }
}

#pragma endregion