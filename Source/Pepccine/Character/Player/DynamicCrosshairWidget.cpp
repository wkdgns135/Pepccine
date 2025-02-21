#include "Character/Player/DynamicCrosshairWidget.h"

void UDynamicCrosshairWidget::UpdateCrosshairSize(const float Speed)
{
  if (!LineTop || !LineBottom || !LineLeft || !LineRight)
    return;

  UE_LOG(LogTemp, Log, TEXT("UpdateCrosshairSize [%f]"), Speed);

  float Offset = FMath::Clamp(Speed * 0.1f, 0, 50.0f);

  LineTop->SetRenderTranslation(FVector2D(0, -Offset));
  LineBottom->SetRenderTranslation(FVector2D(0, Offset));
  LineLeft->SetRenderTranslation(FVector2D(-Offset, 0));
  LineRight->SetRenderTranslation(FVector2D(Offset, 0));
}