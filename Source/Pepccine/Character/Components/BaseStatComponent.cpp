// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/BaseStatComponent.h"

UBaseStatComponent::UBaseStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBaseStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBaseStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

