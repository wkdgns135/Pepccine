// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HealthWidget.generated.h"

UCLASS()
class PEPCCINE_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthPrograssBar;

	UFUNCTION(BlueprintCallable, Category = "UI|Health")
	void UpdateHealthVolume(const float Health, const float MaxHealth);
};

