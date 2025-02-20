// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerStatComponent.h"
#include "PepCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class PEPCCINE_API APepCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APepCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UPlayerStatComponent* PlayerStatComponent;

	UFUNCTION()
	void Move(const FInputActionValue& Value);
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
	void Crouching();
	UFUNCTION()
	void Reload();
	UFUNCTION()
	void Interactive();
	UFUNCTION()
	void OpenInventory();
	UFUNCTION()
	void SwapItem(const FInputActionValue& value);

private:
	float CameraArmLength = 300.0f;
	
	bool bIsfire = false;
	bool bIsJumping = false;
	bool bIsCrouching = false;
	bool bIsSpringting = false;
	bool bIsReloading = false;
	bool bIsInteracting = false;
	bool bIsInventoryOpened = false;

	void DefineCharacterMovement();
};
