#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "StaminaWidget.generated.h"

UCLASS()
class PEPCCINE_API UStaminaWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaPrograssBar;
	
	UFUNCTION(BlueprintCallable, Category = "UI|Stamina")
	void UpdateStaminaVolumne(const float stamina, const float maxStamina);
};
