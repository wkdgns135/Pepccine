// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	void Move(const FInputActionValue& Value);
	void JumpStart();
	void JumpStop();
};
