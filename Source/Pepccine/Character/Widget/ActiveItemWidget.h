#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActiveItemWidget.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class PEPCCINE_API UActiveItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ActiveText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ActiveButton;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ActiveProgressBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CoinsText;

	UFUNCTION(BlueprintCallable, Category = "UI|Active")
	void UpdateActiveItemUI(UTexture2D* ActiveImage, const FString& ActiveName, const FString& Button, const float Cooldown, const float MaxCooldown);
	UFUNCTION(BlueprintCallable, Category = "UI|Active")
	void UpdateCoinUI(int Coins);
};

