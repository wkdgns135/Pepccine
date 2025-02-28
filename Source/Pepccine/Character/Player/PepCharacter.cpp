#include "PepCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "EnhancedInputComponent.h"
#include "Item/PepccineItemManagerComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/Components/CrosshairHUDComponent.h"
#include "Character/Components/PrograssBarHUDComponent.h"
#include "Character/Components/ItemIconHUDComponent.h"
#include "Character/Components/CollisionRadarComponent.h"
#include "Character/Components/InventoryComponent.h"

#include "Character/Controller/PepccineCameraModifier.h"

#include "GameFramework/SpringArmComponent.h"
#include "Character/Animation/PepccineMontageComponent.h"
#include "Character/Data/ActorInfo.h"
#include "Components/WidgetComponent.h"
#include "Item/PepccineDropItem.h"
#include "Item/Passive/PepccinePassiveItemData.h"
#include "Kismet/GameplayStatics.h"

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
	ItemIconComponent = CreateDefaultSubobject<UItemIconHUDComponent>(TEXT("ItemIconComponent"));

	EnhancedRadarComponent = CreateDefaultSubobject<UCollisionRadarComponent>(TEXT("EnhancedRadarComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	PepccineMontageComponent = CreateDefaultSubobject<UPepccineMontageComponent>(TEXT("MontageComponent"));
	ItemManagerComponent = CreateDefaultSubobject<UPepccineItemManagerComponent>(TEXT("ItemManagerComponent"));
}

void APepCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeCharacterMovement();
	AddObservers();
}

void APepCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckSprinting();
	CheckRolling(DeltaTime);
}

// Initialize Character Status
#pragma region
void APepCharacter::InitializeCharacterMovement() const
{
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		if (!PlayerStatComponent)
		{
			return;
		}
		MovementComponent->GetNavAgentPropertiesRef().bCanCrouch = true;
		MovementComponent->MaxWalkSpeed = PlayerStatComponent->GetCurrentStats().MovementStats.MovementSpeed;
		MovementComponent->JumpZVelocity = PlayerStatComponent->GetCurrentStats().MovementStats.JumpZVelocity;
	}
}
#pragma endregion

// Tick Method
#pragma region
void APepCharacter::CheckSprinting()
{
	if (!PlayerStatComponent)
	{
		return;
	}

	if (bIsSprinting)
	{
		if (!PlayerStatComponent->DecreaseStamina(0.25))
		{
			bIsSprinting = false;
			return;
		}
		SetCharacterSpeed(PlayerStatComponent->GetCurrentStats().MovementStats.SprintSpeed);
	}
	else
	{
		SetCharacterSpeed(PlayerStatComponent->GetCurrentStats().MovementStats.MovementSpeed);
	}
}

void APepCharacter::CheckRolling(float DeltaTime)
{
	if (bIsRolling && PlayerStatComponent)
	{
		float RollTime = PlayerStatComponent->GetCurrentStats().MovementStats.RollElapsedTime;
		RollTime += DeltaTime;

		float RollSpeed = PlayerStatComponent->GetCurrentStats().MovementStats.RollingDistance;
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
		PlayerStatComponent->OnHealthChanged.AddDynamic(this, &APepCharacter::OnHealthChanged);
	}

	if (EnhancedRadarComponent)
	{
		EnhancedRadarComponent->OnActorDetectedEnhanced.AddDynamic(this, &APepCharacter::OnActorDetectedEnhanced);
	}
}

void APepCharacter::OnHealthChanged(const float NewHealth, const float MaxHealth)
{
	if (!PrograssBarComponent)
	{
		return;
	}

	PrograssBarComponent->SetHealth(NewHealth, MaxHealth);
}

void APepCharacter::OnStaminaChanged(float NewStamina, float MaxStamina)
{
	if (!PrograssBarComponent)
	{
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Stamina Updated: %f / %f"), NewStamina, MaxStamina);
	PrograssBarComponent->SetStamina(NewStamina, MaxStamina);
}

void APepCharacter::OnActorDetectedEnhanced(FDetectedActorList& DetectedActors)
{
	if (DetectedActors.DetectedActors.Num() == 0)
	{
		return;
	}

	const FDetectedActorInfo* MinDistActor = Algo::MinElement(DetectedActors.DetectedActors,
	                                                          [](const FDetectedActorInfo& InfoA,
	                                                             const FDetectedActorInfo& InfoB)
	                                                          {
		                                                          return InfoA.Distance < InfoB.Distance;
	                                                          });

	if (APepccineDropItem* DropItem = Cast<APepccineDropItem>(MinDistActor->Actor))
	{
		DropItem->ShowInteractWidget(true);

		if (CurrentDropItem && CurrentDropItem != DropItem)
		{
			CurrentDropItem->ShowInteractWidget(false);
		}

		CurrentDropItem = DropItem;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Number of DectedActor: %d"), DetectedActors.DetectedActors.Num());
	//UE_LOG(LogTemp, Warning, TEXT("Detected Actors: [%s]"), *MinDistActor->Actor->GetName());
	//UE_LOG(LogTemp, Warning, TEXT("Detected Actors Loc: [%f]"), MinDistActor->Distance);
}
#pragma endregion

// Action
#pragma region
void APepCharacter::Move(const FInputActionValue& Value)
{
	if (bIsRolling)
	{
		return;
	}

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
	if (bIsRolling)
	{
		return;
	}

	Super::Jump();

	Jump();
}

void APepCharacter::JumpStop()
{
	Super::StopJumping();

	StopJumping();
}

void APepCharacter::UseItem()
{
	UE_LOG(LogTemp, Log, TEXT("UseItem!"));
}

void APepCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void APepCharacter::StartSprint(const FInputActionValue& value)
{
	if (bIsRolling)
	{
		return;
	}

	if (bIsRollable)
	{
		SprintHoldStartTime = GetWorld()->GetTimeSeconds();
	}

	if (!bIsSprintable)
	{
		return;
	}
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

	SprintHoldStartTime = 0.0f;
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
	if (!GetCharacterMovement() || bIsRolling || !PlayerStatComponent || GetCharacterMovement()->IsFalling())
	{
		return;
	}
	
	// 임시
	//TriggerCameraShake();

	bIsRolling = true;
	RollDirection = GetRollDirection();

	if (!PlayerStatComponent->DecreaseStaminaByPercentage(30))
	{
		bIsRolling = false;
		return;
	}

	PepccineMontageComponent->Roll(GetRollDirection(), GetActorRotation());
	GetWorldTimerManager().SetTimer(RollTimerHandle, this, &APepCharacter::EndRoll, 0.5f, false);
}

void APepCharacter::EndRoll()
{
	bIsRolling = false;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = PlayerStatComponent->GetCurrentStats().MovementStats.MovementSpeed;
	}
}

FVector APepCharacter::GetRollDirection()
{
	if (!PlayerStatComponent)
	{
		return FVector::ZeroVector;
	}

	FVector Velocity = GetCharacterMovement()->Velocity;

	if (!Velocity.IsNearlyZero())
	{
		RollDirection = Velocity.GetSafeNormal() * PlayerStatComponent->GetCurrentStats().MovementStats.RollingDistance;
	}
	else
	{
		RollDirection = GetActorForwardVector() * PlayerStatComponent->GetCurrentStats().MovementStats.RollingDistance;
	}

	return RollDirection;
}

void APepCharacter::Crouching()
{
	if (!GetCharacterMovement() || bIsRolling || !PlayerStatComponent)
	{
		return;
	}

	bIsCrouching = GetCharacterMovement()->IsCrouching();

	if (bIsCrouching)
	{
		UnCrouch();
		GetCharacterMovement()->MaxWalkSpeed = PlayerStatComponent->GetCurrentStats().MovementStats.MovementSpeed;
	}
	else
	{
		Crouch();
		GetCharacterMovement()->MaxWalkSpeed = PlayerStatComponent->GetCurrentStats().MovementStats.CrouchSpeed;
	}
}

void APepCharacter::Reload()
{
	UE_LOG(LogTemp, Log, TEXT("Reload!"));

	if (bIsReloading)
	{
		bIsReloading = false;
	}
	else
	{
		ItemManagerComponent->ReloadWeapon();
		PepccineMontageComponent->Reloading();
		bIsReloading = true;
	}
}

void APepCharacter::Interactive()
{
	// 아이템 인벤토리에 추가
	if (CurrentDropItem)
	{
		CurrentDropItem->PickUpItem(ItemManagerComponent);

		if (CurrentDropItem->IsA(UPepccinePassiveItemData::StaticClass()))
		{
			// 패시브 아이템
			const UPepccinePassiveItemData* DropItem = Cast<UPepccinePassiveItemData>(CurrentDropItem->GetDropItemData());
			InventoryComponent->AddItem(DropItem->IconTexture, DropItem->GetDisplayName(), DropItem->GetDescription());

			TArray<FPepccineCharacterStatModifier> CharacterStatModifiers = DropItem->GetCharacterStatModifiers();
			for (const FPepccineCharacterStatModifier& Modifier : CharacterStatModifiers)
			{
				EPepccineCharacterStatName CharacterStatName = Modifier.CharacterStatName;
				EPepccineStatModifyType ModifyType = Modifier.StatModifierDefault.StatModifyType;
				float Amount = Modifier.StatModifierDefault.StatModifyValue;

				FString StatName = UEnum::GetValueAsString(CharacterStatName);
				UE_LOG(LogTemp, Warning, TEXT("CharacterStatName: %s"), *StatName);

				FString MT = UEnum::GetValueAsString(ModifyType);
				UE_LOG(LogTemp, Warning, TEXT("ModifyType: %s"), *MT);
				UE_LOG(LogTemp, Warning, TEXT("Amount: %f"), Amount);
			}

			TArray<FPepccineWeaponStatModifier> WeaponStatModifiers = DropItem->GetWeaponStatModifiers();
			for (const FPepccineWeaponStatModifier& Modifier : WeaponStatModifiers)
			{
				EPepccineWeaponItemType WeaponItemType = Modifier.WeaponItemType;
				EPepccineWeaponStatName WeaponItemStatName = Modifier.WeaponItemStatName;
				float Amount = Modifier.StatModifierDefault.StatModifyValue;

				FString WeaponType = UEnum::GetValueAsString(WeaponItemType);
				UE_LOG(LogTemp, Warning, TEXT("WeaponItemType: %s"), *WeaponType);

				FString WeaponStatName = UEnum::GetValueAsString(WeaponItemStatName);
				UE_LOG(LogTemp, Warning, TEXT("WeaponItemStatName: %s"), *WeaponStatName);
				UE_LOG(LogTemp, Warning, TEXT("Amount: %f"), Amount);
			}

			TArray<FPepccineCharacterFeature> CharacterFeatures = DropItem->CharacterFeatures;
			for (const FPepccineCharacterFeature& Feature : CharacterFeatures)
			{
				switch (Feature.CharacterFeatureName)
				{
				case EPepccineCharacterFeatureName::EPCFN_Roll:
					break;
				case EPepccineCharacterFeatureName::EPCFN_Sprint:
					break;
				}
			}
		}
		else if (CurrentDropItem->IsA(UPepccineWeaponItemData::StaticClass()))
		{
			// 무기류 아이템
			const UPepccineWeaponItemData* DropItem = Cast<UPepccineWeaponItemData>(CurrentDropItem->GetDropItemData());
			// UTexture2D* MainWeaponImage, UTexture2D* SubWeaponImage, const FString& WeaponName, const int32 Ammo, const int32 MaxAmmo)

			if (DropItem->GetWeaponItemType() == EPepccineWeaponItemType::EPWIT_Main)
			{
				// ItemIconComponent->UpdateMainWeaponUI();
			}
			else if (DropItem->GetWeaponItemType() == EPepccineWeaponItemType::EPWIT_Sub)
			{
				// ItemIconComponent->UpdateSubWeaponUI();
			}
		}
		else if (CurrentDropItem->IsA(UPepccineWeaponItemData::StaticClass()))
		{
			// 액티브 아이템
			const UPepccineWeaponItemData* DropItem = Cast<UPepccineWeaponItemData>(CurrentDropItem->GetDropItemData());
		}
	}

	// 스텟연산 (저장 구조체)
	// 갖고있는 모든 패시브 아이템 적용 (캐릭터 스텟)
	// 1. 케릭터 스탯 연산 선행
	// 2. 무기 스탯 반영
	// 3. PlayerStatComponent->AttackDamage 

	// 갖고있는 무기 스텟 (총 스텟)

	// Delay 있는 상호작용 전용
	if (bIsInteracting)
	{
	}
	else
	{
	}
}

void APepCharacter::OpenInventory()
{
	if (bIsRolling || !InventoryComponent)
	{
		return;
	}

	bIsInventoryOpened = !bIsInventoryOpened;
	InventoryComponent->ToggleInventory();

	if (PlayerController)
	{
		if (bIsInventoryOpened)
		{
			PlayerController->bShowMouseCursor = true;
			PlayerController->SetInputMode(FInputModeGameAndUI());
		}
		else
		{
			PlayerController->bShowMouseCursor = false;
			PlayerController->SetInputMode(FInputModeGameOnly());
		}
	}
}

void APepCharacter::SwapItem(const FInputActionValue& value)
{
	float ScrollValue = value.Get<float>();

	if (ScrollValue > 0.0f)
	{
		ItemManagerComponent->SwapWeapon(EPepccineWeaponItemType::EPWIT_Main);
	}
	else if (ScrollValue < 0.0f)
	{
		ItemManagerComponent->SwapWeapon(EPepccineWeaponItemType::EPWIT_Sub);
	}
}

void APepCharacter::Fire()
{
	if (bIsRolling)
	{
		return;
	}
	
	PepccineMontageComponent->Fire();
	// 무기 데미지
	
	ItemManagerComponent->FireWeapon(PlayerStatComponent->GetCurrentStats().CombatStats.AttackDamage);
}

void APepCharacter::ZoomIn()
{
	if (bIsRolling)
	{
		return;
	}

	bIsZooming = true;

	ToggleCameraView();

	if (!CrosshairComponent)
	{
		return;
	}
	CrosshairComponent->ShowCrosshair();
}

void APepCharacter::ZoomOut()
{
	UE_LOG(LogTemp, Log, TEXT("ZoomOut!"));

	bIsZooming = false;

	ToggleCameraView();

	if (!CrosshairComponent)
	{
		return;
	}
	CrosshairComponent->HideCrosshair();
}

void APepCharacter::ToggleCameraView()
{
	if (!PlayerController)
	{
		return;
	}

	bIsFirstPersonView = !bIsFirstPersonView;

	FirstPersonCamera->SetActive(bIsFirstPersonView);
	ThirdPersonCamera->SetActive(!bIsFirstPersonView);
}

void APepCharacter::TriggerCameraShake()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC && PC->PlayerCameraManager)
	{
		UPepccineCameraModifier* Modifier
			= Cast<UPepccineCameraModifier>(
				PC->PlayerCameraManager->FindCameraModifierByClass(UPepccineCameraModifier::StaticClass()));
		if (!Modifier)
		{
			Modifier = Cast<UPepccineCameraModifier>(
				PC->PlayerCameraManager->AddNewCameraModifier(UPepccineCameraModifier::StaticClass()));
		}

		if (Modifier)
		{
			Modifier->StartShake(10.0f, 20.0f, 0.5f);
		}
	}
}
#pragma endregion

// Key Mapping
#pragma region
void APepCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInput)
	{
		return;
	}

	PlayerController = Cast<APepccinePlayerController>(GetController());
	if (!PlayerController)
	{
		return;
	}

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
			ETriggerEvent::Started,
			this,
			&APepCharacter::Reload
		);
	}

	// Interactive: E
	if (PlayerController->InteractiveAction)
	{
		EnhancedInput->BindAction(
			PlayerController->InteractiveAction,
			ETriggerEvent::Started,
			this,
			&APepCharacter::Interactive
		);
	}

	// Inventory: Tab
	if (PlayerController->InventoryAction)
	{
		EnhancedInput->BindAction(
			PlayerController->InventoryAction,
			ETriggerEvent::Started,
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
			ETriggerEvent::Triggered,
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

// Test Code
#pragma region
void APepCharacter::TestApplyStatModifier()
{
	if (!PlayerStatComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerStatComponent가 없습니다!"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("== 1. 스탯 적용 =="));
	UE_LOG(LogTemp, Log, TEXT("현재 공격력: %f"), PlayerStatComponent->GetCurrentStats().CombatStats.AttackDamage);
	UE_LOG(LogTemp, Log, TEXT("적용 예정: 10.0f, 1.2f"));

	FStatModifier AttackModifier(EPepccineCharacterStatName::EPCSN_AttackDamage, 10.0f, 1.2f);
	PlayerStatComponent->ApplyStatModifier(AttackModifier);

	UE_LOG(LogTemp, Log, TEXT("== 2. 스탯 적용 =="));
	UE_LOG(LogTemp, Log, TEXT("현재 공격력: %f"), PlayerStatComponent->GetCurrentStats().CombatStats.AttackDamage);
	UE_LOG(LogTemp, Log, TEXT("적용 예정: 50.0f, 1.1f"));

	FStatModifier AttackModifierA(EPepccineCharacterStatName::EPCSN_AttackDamage, 50.0f, 1.1f);
	PlayerStatComponent->ApplyStatModifier(AttackModifierA);

	UE_LOG(LogTemp, Log, TEXT("== 3. 스탯 적용 =="));
	UE_LOG(LogTemp, Log, TEXT("현재 공격력: %f"), PlayerStatComponent->GetCurrentStats().CombatStats.AttackDamage);
}

void APepCharacter::TestRemoveStatModifier()
{
	if (!PlayerStatComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerStatComponent가 없습니다!"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("== 1. 스탯 제거 =="));
	UE_LOG(LogTemp, Log, TEXT("현재 공격력: %f"), PlayerStatComponent->GetCurrentStats().CombatStats.AttackDamage);
	UE_LOG(LogTemp, Log, TEXT("적용 예정: 10.0f, 1.2f"));

	FStatModifier AttackModifier(EPepccineCharacterStatName::EPCSN_AttackDamage, 50.0f, 1.1f);
	PlayerStatComponent->RemoveStatModifier(AttackModifier);

	UE_LOG(LogTemp, Log, TEXT("== 2. 스탯 제거 =="));
	UE_LOG(LogTemp, Log, TEXT("현재 공격력: %f"), PlayerStatComponent->GetCurrentStats().CombatStats.AttackDamage);
	UE_LOG(LogTemp, Log, TEXT("적용 예정: 50.0f, 1.1f"));

	FStatModifier AttackModifierA(EPepccineCharacterStatName::EPCSN_AttackDamage, 10.0f, 1.2f);
	PlayerStatComponent->RemoveStatModifier(AttackModifierA);

	UE_LOG(LogTemp, Log, TEXT("== 3. 스탯 제거 =="));
	UE_LOG(LogTemp, Log, TEXT("현재 공격력: %f"), PlayerStatComponent->GetCurrentStats().CombatStats.AttackDamage);
}
#pragma endregion