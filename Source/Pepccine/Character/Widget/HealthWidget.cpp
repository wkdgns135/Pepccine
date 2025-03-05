#include "Character/Widget/HealthWidget.h"

void UHealthWidget::UpdateHealthVolume(const float Health, const float MaxHealth)
{
	if (!HealthPrograssBar || MaxHealth <= 0.0f) return;

	const float HealthPercent = Health / MaxHealth;

	FLinearColor NewColor = FMath::Lerp(FLinearColor::Red, FLinearColor::Green, HealthPercent);
	HealthPrograssBar->SetFillColorAndOpacity(NewColor);
	HealthPrograssBar->SetPercent(HealthPercent);
}
