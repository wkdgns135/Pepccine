#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MouseSensitivityWidget.generated.h"

class USlider;
class UTextBlock;
class UButton;
class APepCharacter;

UCLASS()
class PEPCCINE_API UMouseSensitivityWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	USlider* SensitivitySlider;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SensitivityText;

	UPROPERTY(meta = (BindWidget))
	UButton* MouseBackButton;

	UFUNCTION()
	void OnSensitivityChanged(float Value);

private:
	UPROPERTY()
	APepCharacter* PlayerCharacter;
};
