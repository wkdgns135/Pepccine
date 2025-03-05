// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PepccinePlayerHUD.generated.h"

class UMiniMapWidget;

UCLASS()
class PEPCCINE_API APepccinePlayerHUD : public AHUD
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UMiniMapWidget> MiniMapWidgetClass;
	UPROPERTY();
	UMiniMapWidget* MiniMapWidgetInstance;
	
protected:
	virtual void BeginPlay() override;
};
