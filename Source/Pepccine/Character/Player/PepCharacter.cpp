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
#include "Character/Animation/PepccineHitReactionComponent.h"
#include "Character/Components/BattleComponent.h"
#include "Character/Data/ActorInfo.h"
#include "Components/WidgetComponent.h"
#include "Item/PepccineDropItem.h"
#include "Item/Passive/PepccinePassiveItemData.h"
#include "Monster/Class/ZombieGirl.h"

APepCharacter::APepCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmCompThird = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmThird"));
	SpringArmCompThird->SetupAttachment(RootComponent);
	SpringArmCompThird->TargetArmLength = CameraArmLength;
	SpringArmCompThird->bUsePawnControlRotation = true;

	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->SetupAttachment(SpringArmCompThird, USpringArmComponent::SocketName);
	ThirdPersonCamera->bUsePawnControlRotation = false;

	SpringArmCompFirst = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmFirst"));
	SpringArmCompFirst->SetupAttachment(RootComponent);
	SpringArmCompFirst->TargetArmLength = CameraArmLength;
	SpringArmCompFirst->bUsePawnControlRotation = true;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(SpringArmCompFirst);
	FirstPersonCamera->bUsePawnControlRotation = false;

	PlayerStatComponent = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("PlayerStatComponent"));
	CrosshairComponent = CreateDefaultSubobject<UCrosshairHUDComponent>(TEXT("CrosshairHUDComponent"));
	PrograssBarComponent = CreateDefaultSubobject<UPrograssBarHUDComponent>(TEXT("PrograssBarComponent"));
	ItemIconComponent = CreateDefaultSubobject<UItemIconHUDComponent>(TEXT("ItemIconComponent"));

	EnhancedRadarComponent = CreateDefaultSubobject<UCollisionRadarComponent>(TEXT("EnhancedRadarComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	PepccineMontageComponent = CreateDefaultSubobject<UPepccineMontageComponent>(TEXT("MontageComponent"));
	ItemManagerComponent = CreateDefaultSubobject<UPepccineItemManagerComponent>(TEXT("ItemManagerComponent"));

	HitReactionComponent = CreateDefaultSubobject<UPepccineHitReactionComponent>(TEXT("HitReactionComponent"));
	BattleComponent = CreateDefaultSubobject<UBattleComponent>(TEXT("BattleComponent"));
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

	if (BattleComponent)
	{
		BattleComponent->OnCharacterHited.AddDynamic(this, &APepCharacter::OnPlayerHit);
	}
}

void APepCharacter::OnPlayerHit(AActor* DamageCauser, float DamageAmount, const FHitResult& HitResult)
{
	if (bIsRolling) return;

	PlayerStatComponent->DecreaseHealth(DamageAmount);
	
	FName HitBoneName = HitResult.BoneName;
	FVector HitDirection = HitResult.ImpactNormal;
	
	HitReactionComponent->HitReaction("Spine", HitDirection);
}

void APepCharacter::OnHealthChanged(const float NewHealth, const float MaxHealth)
{
	if (!PrograssBarComponent)
	{
		return;
	}
	else if (NewHealth == 0)
	{
		Dead();
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
void APepCharacter::Dead()
{
	if (!bIsPlayerAlive) return;
	bIsPlayerAlive = false;
	
	if (APepccinePlayerController* PepccinePlayerController = Cast<APepccinePlayerController>(PlayerController))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Dead"));
		// 플레이어 입력 차단
		PepccinePlayerController->DisableInput(PepccinePlayerController);
		
		// 컨트롤러 회전 입력 차단
		PepccinePlayerController->SetIgnoreLookInput(true);
		PepccinePlayerController->SetIgnoreMoveInput(true);
	}
	
	if (GetCharacterMovement())
	{
		UE_LOG(LogTemp, Warning, TEXT("Disable Movement"));
		// 이동 멈추기
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->SetComponentTickEnabled(false);
	}
	
	PepccineMontageComponent->Death();
}

void APepCharacter::Move(const FInputActionValue& Value)
{
	if (bIsRolling | !bIsPlayerAlive)
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
	if (bIsRolling | !bIsPlayerAlive)
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
	if (!bIsPlayerAlive) return;
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
	if (bIsRolling | !bIsPlayerAlive)
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
	if (!GetCharacterMovement() || bIsRolling || !PlayerStatComponent || GetCharacterMovement()->IsFalling() | !bIsPlayerAlive)
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
	if (!GetCharacterMovement() || bIsRolling || !PlayerStatComponent | !bIsPlayerAlive)
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
	if (!bIsPlayerAlive) return;
	UE_LOG(LogTemp, Log, TEXT("Reload!"));

	//HitReactionComponent->EnterRagdoll(5);

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
	if (!bIsPlayerAlive || !PlayerStatComponent) return;
	
	// 아이템 인벤토리에 추가
	if (CurrentDropItem)
	{
		CurrentDropItem->PickUpItem(ItemManagerComponent);
		if (UPepccinePassiveItemData* PassiveItem = Cast<UPepccinePassiveItemData>(CurrentDropItem->GetDropItemData()))
		{
			// 패시브 아이템
			InventoryComponent->AddItem(PassiveItem->GetIconTexture(), PassiveItem->GetDisplayName(), PassiveItem->GetDescription(), PlayerStatComponent->PrintStats());

			TArray<FPepccineCharacterStatModifier> CharacterStatModifiers = PassiveItem->GetCharacterStatModifiers();
			for (const FPepccineCharacterStatModifier& Modifier : CharacterStatModifiers)
			{
				float Amount = Modifier.StatModifierDefault.StatModifyValue;
				//UE_LOG(LogTemp, Warning, TEXT("Amount: %f"), Amount);

				EPepccineCharacterStatName CharacterStatName = Modifier.CharacterStatName;
				FString StatName = UEnum::GetValueAsString(CharacterStatName);
				//UE_LOG(LogTemp, Warning, TEXT("CharacterStatName: %s"), *StatName);

				EPepccineStatModifyType ModifyType = Modifier.StatModifierDefault.StatModifyType;
				FString MT = UEnum::GetValueAsString(ModifyType);
				//UE_LOG(LogTemp, Warning, TEXT("ModifyType: %s"), *MT);

				switch (ModifyType)
				{
				case EPepccineStatModifyType::EPSMT_Add:
					{
						//UE_LOG(LogTemp, Warning, TEXT("EPSMT_Add: %f"), Amount);
						FStatModifier AddStatModifier(CharacterStatName, Amount, 1.0f);
						PlayerStatComponent->ApplyStatModifier(AddStatModifier);
						break;
					}
				case EPepccineStatModifyType::EPSMT_Multiply:
					{
						//UE_LOG(LogTemp, Warning, TEXT("EPSMT_Multiply: %f"), Amount);
						FStatModifier MulStatModifier(CharacterStatName, 0.0f, Amount);
						PlayerStatComponent->ApplyStatModifier(MulStatModifier);
						break;
					}
				}
			}

			TArray<FPepccineCharacterFeature> CharacterFeatures = PassiveItem->GetCharacterFeatures();
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
		else if (UPepccineWeaponItemData* WeaponItem = Cast<UPepccineWeaponItemData>(CurrentDropItem->GetDropItemData()))
		{
			// 무기류 아이템
			UpdateWeaponUI();
		}
		/*
		else if (CurrentDropItem->IsA(UPepccineWeaponItemData::StaticClass()))
		{
			// 액티브 아이템
			const UPepccineWeaponItemData* DropItem = Cast<UPepccineWeaponItemData>(CurrentDropItem->GetDropItemData());
		}
		*/
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

void APepCharacter::UpdateWeaponUI()
{
	if (!ItemManagerComponent || !ItemIconComponent) return;

	// 주무기 정보
	UPepccineWeaponItemData* MainWeaponData = ItemManagerComponent->GetWeaponItemData(EPepccineWeaponItemType::EPWIT_Main);
	FString MainWeaponName = MainWeaponData ? MainWeaponData->GetDisplayName() : FString("None");
	int32 MainWeaponAmmo = MainWeaponData ? MainWeaponData->GetWeaponItemStats().MagazineAmmo : 0;
	int32 MainWeaponMaxAmmo = MainWeaponData ? MainWeaponData->GetWeaponItemStats().MagazineSize : 0;
	UTexture2D* MainWeaponImage = MainWeaponData ? MainWeaponData->GetIconTexture() : nullptr;

	// 보조무기 정보
	UPepccineWeaponItemData* SubWeaponData = ItemManagerComponent->GetWeaponItemData(EPepccineWeaponItemType::EPWIT_Sub);
	FString SubWeaponName = SubWeaponData ? SubWeaponData->GetDisplayName() : FString("None");
	int32 SubWeaponAmmo = SubWeaponData ? SubWeaponData->GetWeaponItemStats().MagazineAmmo : 0;
	int32 SubWeaponMaxAmmo = SubWeaponData ? SubWeaponData->GetWeaponItemStats().MagazineSize : 0;
	UTexture2D* SubWeaponImage = SubWeaponData ? SubWeaponData->GetIconTexture() : nullptr;

	// 현재 장착된 무기가 주무기인지 확인
	bool bIsMainWeaponEquipped = ItemManagerComponent->GetEquippedWeaponItemData()->GetWeaponItemType() == EPepccineWeaponItemType::EPWIT_Main;

	// WeaponWidget 업데이트
	ItemIconComponent->SetWeaponItem(
		MainWeaponImage,
		SubWeaponImage,
		bIsMainWeaponEquipped ? MainWeaponName : SubWeaponName,
		bIsMainWeaponEquipped ? MainWeaponAmmo : SubWeaponAmmo,
		bIsMainWeaponEquipped ? MainWeaponMaxAmmo : SubWeaponMaxAmmo,
		bIsMainWeaponEquipped
	);
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
	if (!bIsPlayerAlive) return;
	
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

void APepCharacter::StopFire()
{
	bIsFiring = false;
}

void APepCharacter::Fire()
{
	if (bIsRolling | !bIsPlayerAlive) return;

	bIsFiring = true;
	PepccineMontageComponent->Fire();
	ItemManagerComponent->FireWeapon(PlayerStatComponent->GetCurrentStats().CombatStats.AttackDamage);
}

void APepCharacter::ZoomIn()
{
	if (bIsRolling | !bIsPlayerAlive) return;

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

void APepCharacter::ShowMenu()
{
	if (!PlayerController) return;
	PlayerController->ToggleExitMenu();
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

	if (PlayerController->FireAction)
	{
		EnhancedInput->BindAction(
			PlayerController->FireAction,
			ETriggerEvent::Completed,
			this,
			&APepCharacter::StopFire
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

	// Menu: Escape
	if (PlayerController->MenuAction)
	{
		EnhancedInput->BindAction(
			PlayerController->MenuAction,
			ETriggerEvent::Started,
			this,
			&APepCharacter::ShowMenu
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