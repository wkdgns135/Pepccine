// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player/PlayerStatComponent.h"

UPlayerStatComponent::UPlayerStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 100;
	MovementSpeed = 400;
	AttackDamage = 100;
	CrouchSpeed = MovementSpeed / 2;
	SprintSpeed = MovementSpeed * 2;
	CurrentHealth = MaxHealth;
	HealthDecelerationSpeed = 0;
	Stamina = 0;
	InvincibilityTime = 0;
	Defence = 0;  
	FireRate = 0;
	RollingDistance = 10000.0f;
	JumpZVelocity = 600.0f;
}

void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

