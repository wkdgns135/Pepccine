#include "Character/Widget/ActiveItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UActiveItemWidget::UpdateActiveItemUI(UTexture2D* ActiveImage, const FString& ActiveName, const FString& Button, const float Cooldown, const float MaxCooldown)
{
    ActiveText->SetText(FText::FromString(ActiveName));
    ActiveButton->SetText(FText::FromString(Button));
    
    if (ActiveImage)
    {
        FSlateBrush Brush;
        Brush.SetResourceObject(ActiveImage);
        Brush.ImageSize = FVector2D(100, 100); // 크기 설정

        FProgressBarStyle Style;
        Style.SetBackgroundImage(Brush);

        ActiveProgressBar->SetWidgetStyle(Style);
        ActiveProgressBar->SetPercent(Cooldown / MaxCooldown);
    }
}

void UActiveItemWidget::UpdateCoinUI(const int Coins)
{
    CoinsText->SetVisibility(ESlateVisibility::Visible);
    CoinsText->SetText(FText::Format(FText::FromString(TEXT("Coins: {0}")), FText::AsNumber(Coins)));
}