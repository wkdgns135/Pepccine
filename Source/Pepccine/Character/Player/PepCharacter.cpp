
#include "PepCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "CrosshairHUDComponent.h"
#include "PrograssBarHUDComponent.h"

#include "RadorComponent.h"
#include "CollisionRadarComponent.h"

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

  //RadarComponent = CreateDefaultSubobject<URadorComponent>(TEXT("RadarComponent"));
  //RadarComponent->DetectionClass = AActor::StaticClass();

  EnhancedRadarComponent = CreateDefaultSubobject<UCollisionRadarComponent>(TEXT("EnhancedRadarComponent"));

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
    if (!PlayerStatComponent) return;
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
  CheckRolling(DeltaTime);
}

// Delegate
float APepCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
  OnHealthChanged.Broadcast(0.1f);

  // OnHealthChanged.AddDynamic(this, &AMyCharacter::OnHealthChangedFunction);

  return 0.0f;
}

// Tick Method
#pragma region
void APepCharacter::CheckSprinting()
{
  if (!PlayerStatComponent) return;

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

void APepCharacter::CheckRolling(float DeltaTime)
{
  if (bIsRolling && PlayerStatComponent)
  {
    float RollTime = PlayerStatComponent->RollElapsedTime;
    RollTime += DeltaTime;

    float RollSpeed = PlayerStatComponent->RollingDistance;
    AddMovementInput(RollDirection, RollSpeed * DeltaTime);
  }
}
#pragma endregion

// Observers
#pragma region
void APepCharacter::AddObservers()
{
  if (PlayerStatComponent)
  {
    PlayerStatComponent->AddStaminaObserver(this);
  }
  
  if (RadarComponent)
  {
    RadarComponent->OnActorDetected.AddDynamic(this, &APepCharacter::OnActorDetected);
  }

  if (EnhancedRadarComponent)
  {
    EnhancedRadarComponent->OnActorDetectedEnhanced.AddDynamic(this, &APepCharacter::OnActorDetectedEnhanced);
  }
}

void APepCharacter::OnStaminaChanged(float NewStamina, float MaxStamina)
{
  if (!PrograssBarComponent) return;
  //UE_LOG(LogTemp, Warning, TEXT("Stamina Updated: %f / %f"), NewStamina, MaxStamina);
  PrograssBarComponent->SetStamina(NewStamina, MaxStamina);
}

void APepCharacter::OnActorDetected(AActor* DetectedActor)
{
  if (DetectedActor)
  {
    UE_LOG(LogTemp, Warning, TEXT("OnActorDetected: %s"), *DetectedActor->GetName());
  }
}

void APepCharacter::OnActorDetectedEnhanced(AActor* DetectedActor)
{
  if (DetectedActor)
  {
    UE_LOG(LogTemp, Warning, TEXT("OnActorDetectedEnhanced: %s"), *DetectedActor->GetName());
  }
}
#pragma endregion

// TODO: 분류 세분화 필요
// Action
#pragma region
void APepCharacter::Move(const FInputActionValue& Value)
{
  if (bIsRolling) return;

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
  if (bIsRolling) return;

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
  if (bIsRolling) return;

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
  if (!GetCharacterMovement() || bIsRolling || bIsJumping || !PlayerStatComponent) return;

  bIsRolling = true;
  PlayerStatComponent->RollElapsedTime = 0.0f;
  RollDirection = GetActorForwardVector();

  if (!PlayerStatComponent->DecreaseStaminaByPercentage(30))
  {
    bIsRolling = false;
    return;
  }

  PepccineMontageComponent->Roll();
  GetWorldTimerManager().SetTimer(RollTimerHandle, this, &APepCharacter::EndRoll, 1.0f, false);
}

void APepCharacter::EndRoll()
{
  bIsRolling = false;

  // [임시] 롤 활성시에 해당부분 접근을 못해서 가져옴
  if (GetCharacterMovement())
  {
    GetCharacterMovement()->MaxWalkSpeed = PlayerStatComponent->MovementSpeed;
  }
}

FVector APepCharacter::GetRollDirection()
{
  if (!PlayerStatComponent) return FVector::ZeroVector;

  FVector Velocity = GetCharacterMovement()->Velocity;

  if (!Velocity.IsNearlyZero())
  {
    RollDirection = Velocity.GetSafeNormal() * PlayerStatComponent->RollingDistance;
  }
  else
  {
    RollDirection = GetActorForwardVector() * PlayerStatComponent->RollingDistance;
  }

  return RollDirection;
}

void APepCharacter::Crouching()
{
  if (!GetCharacterMovement() || bIsRolling || !PlayerStatComponent)
    return;

  bIsCrouching = GetCharacterMovement()->IsCrouching();

  if (bIsCrouching)
  {
    UnCrouch();
    GetCharacterMovement()->MaxWalkSpeed = PlayerStatComponent->MovementSpeed;
  }
  else
  {
    Crouch();
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

  if (bIsRolling) return;

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

  if (bIsRolling) return;

  PepccineMontageComponent->Fire();
}

void APepCharacter::ZoomIn()
{
  UE_LOG(LogTemp, Log, TEXT("ZoomIn!"));

  if (bIsRolling) return;

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