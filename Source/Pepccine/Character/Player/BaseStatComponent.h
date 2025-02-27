// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEPCCINE_API UBaseStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBaseStatComponent();

protected:
	float CurrentHealth;
	float BaseMaxHealth;
	float BaseMovementSpeed;
	float BaseAttackDamage;
	
	float BaseHealthDecelerationSpeed;
	float BaseHealthDecelerationAmount;
	
	float CurrentStamina;
	float BaseMaxStamina;
	float BaseStaminaRecoveryRate;
	float BaseStaminaRecoveryTime;
	
	float BaseInvincibilityTime;
	float BaseDefence;
	float BaseFireRate;
	float BaseSprintSpeed;
	float BaseCrouchSpeed;
	float BaseRollingDistance;
	float BaseJumpZVelocity;
	float BaseRollElapsedTime;

private:
	
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
