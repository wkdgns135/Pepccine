// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player/PlayerStatComponent.h"

UPlayerStatComponent::UPlayerStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 0;
	MovementSpeed = 0;
	AttackDamage = 0;
	CurrentHealth = MaxHealth;
	HealthDecelerationSpeed = 0;
	Stamina = 0;
	InvincibilityTime = 0;
	Defence = 0;
	FireRate = 0;
}

void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

