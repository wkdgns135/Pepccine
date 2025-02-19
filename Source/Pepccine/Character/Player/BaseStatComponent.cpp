// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/BaseStatComponent.h"

UBaseStatComponent::UBaseStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentHealth = 0;
	MaxHealth = 0;
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

