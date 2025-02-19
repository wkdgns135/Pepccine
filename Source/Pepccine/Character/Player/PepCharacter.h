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

	/*
	BaseStatComponent {Speed 기타 등등 몬스터와 공유할 부분}
	PlayerStatComponent
	PlayerCharacter {}
	*/

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
	void Swap(const FInputActionValue& value);
};
