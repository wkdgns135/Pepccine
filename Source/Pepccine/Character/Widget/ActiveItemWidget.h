// Fill out your copyright notice in the Description page of Project Settings.

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
	UProgressBar* ActiveProgressBar;

	UFUNCTION(BlueprintCallable, Category = "UI|Active")
	void UpdateWeaponUI(UTexture2D* ActiveImage, const FString& ActiveName, FString& ActiveButton, const int32 Cooldown, const int32 MaxCooldown);
};
