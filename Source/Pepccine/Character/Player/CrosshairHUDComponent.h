// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "CrosshairHUDComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEPCCINE_API UCrosshairHUDComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCrosshairHUDComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Crosshair", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> CrosshairWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD|Crosshair", meta = (AllowPrivateAccess = "true"))
	UUserWidget* CrosshairWidget;

	UFUNCTION(BlueprintCallable)
	void ShowCrosshair();
	UFUNCTION(BlueprintCallable)
	void HideCrosshair();
	UFUNCTION(BlueprintCallable)
	void GetPlayerController();

	UImage* CrosshairImage;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
};
