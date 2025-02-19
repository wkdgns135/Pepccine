// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/BaseStatComponent.h"

UBaseStatComponent::UBaseStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 0;
	CurrentHealth = MaxHealth;
	MovementSpeed = 0;
	AttackDamage = 0;
}

void UBaseStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBaseStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

