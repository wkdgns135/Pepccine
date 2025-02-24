#include "Character/Player/StaminaWidget.h"

void UStaminaWidget::UpdateStaminaVolumne(const float stamina, const float maxStamina)
{
  if (!StaminaPrograssBar || maxStamina <= 0.0f) return;

  float StaminaPercent = stamina / maxStamina;

  StaminaPrograssBar->SetPercent(StaminaPercent);

  // UE_LOG(LogTemp, Log, TEXT("stamina [%f]/[%f] - Percent: %f"), stamina, maxStamina, StaminaPercent);
}
