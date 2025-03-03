// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Widget/HealthWidget.h"

void UHealthWidget::UpdateHealthVolume(const float Health, const float MaxHealth)
{
	if (!HealthPrograssBar || MaxHealth <= 0.0f) return;

	const float HealthPercent = Health / MaxHealth;

	HealthPrograssBar->SetPercent(HealthPercent);
}
