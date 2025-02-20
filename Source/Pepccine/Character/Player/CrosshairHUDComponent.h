// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairHUDComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEPCCINE_API UCrosshairHUDComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCrosshairHUDComponent();

	void ShowCrosshair();
	void HideCrosshair();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = "HUD|Crosshair")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;
	UUserWidget* CrosshairWidget;
	APlayerController* NewController;

	void GetPlayerController();
};
