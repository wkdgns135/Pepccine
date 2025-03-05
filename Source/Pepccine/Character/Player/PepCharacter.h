// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Character/Controller/PepccinePlayerController.h"
#include "Character/Components/PlayerStatComponent.h"
#include "Character/Components/CrosshairHUDComponent.h"
#include "Character/Data/ActorInfo.h"
#include "Character/Interfaces/IStaminaObserver.h"
#include "Item/PepccineDropItem.h"
#include "Monster/Data/MonsterSkill.h"
#include "PepCharacter.generated.h"

struct FClimbObstacleInfo;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UPepccineMontageComponent;
class UPrograssBarHUDComponent;
class UItemIconHUDComponent;
class UInventoryComponent;
class UPepccineItemManagerComponent;
class UBattleComponent;
//class URadorComponent;
class UCollisionRadarComponent;

class UPepccineHitReactionComponent;

UCLASS()
class PEPCCINE_API APepCharacter : public ACharacter, public IIStaminaObserver
{
	GENERATED_BODY()

public:
	APepCharacter();

	// 내부 & 애니매이션 사용
	bool bIsFiring = false;
	bool bIsFirstPersonView = false;
	bool bIsInventoryOpened = false;
	
	bool bIsMoving = false;
	bool bIsZooming = false;
	bool bIsSprinting = false;
	bool bIsClimbing = false;
	bool bIsStunning = false;
	
	bool bIsCrouching = false;
	bool bIsSprintable = true;
	bool bIsReloading = false;
	bool bIsInteracting = false;
	bool bIsRolling = false;
	bool bIsRollable = true;

	bool bIsSwapping = false;
	bool bIsPlayerAlive = true;
	bool bIsMainWeaponEquipped = false;
	bool bIsLoaded = false;
	// 내부 & 애니매이션 사용

	// UE delegate
	UFUNCTION()
	void OnHealthChanged(const float NewHealth, const float MaxHealth);
	// Observer Pattern
	virtual void OnStaminaChanged(float NewStamina, float MaxStamina) override;
	UFUNCTION()
	void OnPlayerHit(AActor* DamageCauser, float DamageAmount, const FHitResult& HitResult, EMonsterSkill SkillType);
	
	void TriggerCameraShake(float Strength, float ShakeTime);
	
	// inline
	FORCEINLINE_DEBUGGABLE bool IsRolling() const { return bIsRolling; }
	FORCEINLINE_DEBUGGABLE bool IsInventoryOpen() const { return bIsInventoryOpened; }
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	APepccinePlayerController* PlayerController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArmCompFirst;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* FirstPersonCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArmCompThird;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* ThirdPersonCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UPlayerStatComponent* PlayerStatComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCrosshairHUDComponent* CrosshairComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UPrograssBarHUDComponent* PrograssBarComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UItemIconHUDComponent* ItemIconComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCollisionRadarComponent* EnhancedRadarComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UInventoryComponent* InventoryComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UPepccineItemManagerComponent* ItemManagerComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UPepccineMontageComponent* PepccineMontageComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UPepccineHitReactionComponent* HitReactionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBattleComponent* BattleComponent;
	
private:
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void OnMovementStopped();
	UFUNCTION()
	void JumpStart();
	UFUNCTION()
	void JumpStop();
	UFUNCTION()
	void UseItem();
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	UFUNCTION()
	void Roll();
	UFUNCTION()
	void EndRoll();
	UFUNCTION()
	void Crouching();
	UFUNCTION()
	void Reload();
	UFUNCTION()
	void Interactive();
	UFUNCTION()
	void UpdateWeaponUI();
	UFUNCTION()
	void OpenInventory();
	UFUNCTION()
	void SwapItem(const FInputActionValue& value);
	UFUNCTION()
	void Fire();
	UFUNCTION()
	void StopFire();
	UFUNCTION()
	void ZoomIn();
	UFUNCTION()
	void ZoomOut();
	UFUNCTION()
	void Dead();
	UFUNCTION()
	void ShowMenu();

	float CameraArmLength = 300.0f;
	float SprintHoldStartTime = 0.0f;
	float SprintHoldThreshold = 0.2f;
	float LooseWeight = 0.0f;
	int ShotStack = 0;
	
	FVector RollDirection;
	FVector GetRollDirection();
	FVector GetKnockbackDirection(AActor* DamageSource, AActor* Victim);
	FVector ClimbTargetLocation;
	
	UPROPERTY()
	APepccineDropItem* CurrentDropItem;

	FTimerHandle RollTimerHandle;

	UFUNCTION()
	void OnActorDetectedEnhanced(FDetectedActorList& DetectedActors);

	void InitializeCharacterMovement() const;
	void ToggleCameraView();
	void AddObservers();

	void SetCharacterSpeed(float Speed);
	void SetCharacterMovement();
	void CheckSprinting();
	void CheckRolling(float DeltaTime);
	void SetWeight();
	void Stumble(AActor* DamageCauser);
	void Climb();

	// TEST CODE
	void TestApplyStatModifier();
	void TestRemoveStatModifier();
};
