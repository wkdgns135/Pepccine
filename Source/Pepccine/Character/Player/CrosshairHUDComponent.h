// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "DynamicCrosshairWidget.h"
#include "CrosshairHUDComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEPCCINE_API UCrosshairHUDComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCrosshairHUDComponent();

	float GetAimSize() const { return AimSize; }
	void SetAimSize(float Size) { AimSize = Size; }

	UPROPERTY(EditAnywhere, Category = "UI")
	UDynamicCrosshairWidget* CrosshairWidget;

	void ShowCrosshair();
	void HideCrosshair();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void UpdateCrosshair(float DeltaTime);

	float AimSize;
	float MaxAimSize;
	float MinAimSize;
};
