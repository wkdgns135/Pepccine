#include "Character/Player/CrosshairHUDComponent.h"
#include "PepCharacter.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"

UCrosshairHUDComponent::UCrosshairHUDComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

  CrosshairWidgetClass = nullptr;
  CrosshairWidget = nullptr;
}

void UCrosshairHUDComponent::BeginPlay()
{
	Super::BeginPlay();

  GetPlayerController();
}

void UCrosshairHUDComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCrosshairHUDComponent::GetPlayerController()
{
  if (CrosshairWidget)
  {
    CrosshairWidget->RemoveFromParent();
    CrosshairWidget = nullptr;
  }

  if (CrosshairWidgetClass)
  {
    CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
    if (CrosshairWidget)
    {
      CrosshairWidget->AddToViewport();
    }

    CrosshairImage = Cast<UImage>(CrosshairWidget->GetWidgetFromName(TEXT("CrosshairImage")));
    if (CrosshairImage)
    {
      CrosshairImage->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
      UE_LOG(LogTemp, Error, TEXT("CrosshairImage is NULL! Check your widget!"));
    }
  }
}

void UCrosshairHUDComponent::ShowCrosshair()
{
  if (CrosshairImage)
  {
    CrosshairImage->SetVisibility(ESlateVisibility::Visible);
  }
}

void UCrosshairHUDComponent::HideCrosshair()
{
  if (CrosshairImage)
  {
    CrosshairImage->SetVisibility(ESlateVisibility::Hidden);
  }
}

