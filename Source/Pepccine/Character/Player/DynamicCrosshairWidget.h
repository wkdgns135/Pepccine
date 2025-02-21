#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "DynamicCrosshairWidget.generated.h"


UCLASS()
class PEPCCINE_API UDynamicCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
  UPROPERTY(meta = (BindWidget))
  UImage* LineTop;

  UPROPERTY(meta = (BindWidget))
  UImage* LineBottom;

  UPROPERTY(meta = (BindWidget))
  UImage* LineLeft;

  UPROPERTY(meta = (BindWidget))
  UImage* LineRight;

  UFUNCTION(BlueprintCallable, Category = "Crosshair")
  void UpdateCrosshairSize(const float Speed);
};
