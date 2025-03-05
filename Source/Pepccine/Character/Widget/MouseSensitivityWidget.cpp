#include "MouseSensitivityWidget.h"

#include "Character/Controller/PepccinePlayerController.h"
#include "Character/Player/PepCharacter.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

void UMouseSensitivityWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const APepccinePlayerController* PlayerController = Cast<APepccinePlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerCharacter = Cast<APepCharacter>(PlayerController->GetPawn());
	}

	if (SensitivitySlider)
	{
		SensitivitySlider->OnValueChanged.AddDynamic(this, &UMouseSensitivityWidget::OnSensitivityChanged);
	}

	if (PlayerCharacter)
	{
		SensitivitySlider->SetValue(PlayerCharacter->MouseSensitivity);
		SensitivityText->SetText(FText::AsNumber(PlayerCharacter->MouseSensitivity));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UMouseSensitivityWidget: Failed to get PlayerCharacter"));
	}
}

void UMouseSensitivityWidget::OnSensitivityChanged(float Value)
{
	if (PlayerCharacter)
	{
		Value = FMath::RoundHalfToZero(Value * 10) / 10.0f;
		PlayerCharacter->MouseSensitivity = Value;
		SensitivityText->SetText(FText::AsNumber(Value));
	}
}