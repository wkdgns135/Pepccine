// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PepCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

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

	void Move(const FInputActionValue& Value);
	void JumpStart();
	void JumpStop();
};
