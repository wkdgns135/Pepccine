// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Pepccine/Character/Controller/PepccinePlayerController.h"
#include "PlayerStatComponent.h"
#include "CrosshairHUDComponent.h"
#include "Character/Interfaces/IStaminaObserver.h"
#include "PepCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class PEPCCINE_API APepCharacter : public ACharacter, public IIStaminaObserver
{
	GENERATED_BODY()

public:
	APepCharacter();

	virtual void OnStaminaChanged(float NewStamina, float MaxStamina) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
	UPepccineMontageComponent* PepccineMontageComponent;

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

	APepccinePlayerController* PlayerController;

private:
	float CameraArmLength = 300.0f;
	
	bool bIsZooming = false;
	bool bIsJumping = false;
	bool bIsCrouching = false;
	bool bIsSprinting = false;
	bool bIsSprintable = true;
	bool bIsReloading = false;
	bool bIsInteracting = false;
	bool bIsInventoryOpened = false;
	bool bIsRolling = false;
	bool bIsRollable = true;
	bool bIsMoving = false;

	bool bIsFirstPersonView = false;

	float SprintHoldStartTime = 0.0f;
	float SprintHoldThreshold = 0.2f;

	FTimerHandle RollTimerHandle;

	void InitializeCharacterMovement();
	void InitializeCharacterCamera();
	void ToggleCameraView();
	void UpdateHUD();
	void AddObservers();

	void SetCharacterSpeed(float Speed);
	void CheckSprinting();

	FVector GetRollDirection();
};
