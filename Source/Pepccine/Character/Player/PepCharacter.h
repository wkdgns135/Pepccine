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
#include "PepCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UPepccineMontageComponent;
class UPrograssBarHUDComponent;
class UItemIconHUDComponent;
class UInventoryComponent;
class UPepccineItemManagerComponent;
//class URadorComponent;
class UCollisionRadarComponent;

UCLASS()
class PEPCCINE_API APepCharacter : public ACharacter, public IIStaminaObserver
{
	GENERATED_BODY()

public:
	APepCharacter();

	bool bIsFirstPersonView = false;
	bool bIsInventoryOpened = false;
	
	bool bIsMoving = false;
	bool bIsZooming = false;
	bool bIsSprinting = false;
	
	bool bIsCrouching = false;
	bool bIsSprintable = true;
	bool bIsReloading = false;
	bool bIsInteracting = false;
	bool bIsRolling = false;
	bool bIsRollable = true;

	// UE delegate
	UFUNCTION()
	void OnHealthChanged(const float NewHealth, const float MaxHealth);
	// Observer Pattern
	virtual void OnStaminaChanged(float NewStamina, float MaxStamina) override;
	
	void TriggerCameraShake();
	
	// inline
	FORCEINLINE_DEBUGGABLE bool IsRolling() const { return bIsRolling; }
	FORCEINLINE_DEBUGGABLE bool IsInventoryOpen() const { return bIsInventoryOpened; }
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	APepccinePlayerController* PlayerController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* FirstPersonCamera;

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
	void OpenInventory();

	UFUNCTION()
	void SwapItem(const FInputActionValue& value);

	UFUNCTION()
	void Fire();
	UFUNCTION()
	void ZoomIn();
	UFUNCTION()
	void ZoomOut();

	float CameraArmLength = 300.0f;
	float SprintHoldStartTime = 0.0f;
	float SprintHoldThreshold = 0.2f;

	FVector RollDirection;
	
	UPROPERTY()
	APepccineDropItem* CurrentDropItem;

	FTimerHandle RollTimerHandle;

	UFUNCTION()
	void OnActorDetectedEnhanced(FDetectedActorList& DetectedActors);

	void InitializeCharacterMovement() const;
	void ToggleCameraView();
	void AddObservers();

	void SetCharacterSpeed(float Speed);
	void CheckSprinting();
	void CheckRolling(float DeltaTime);

	FVector GetRollDirection();

	// TEST CODE
	void TestApplyStatModifier();
	void TestRemoveStatModifier();
};
