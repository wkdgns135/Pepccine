// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseStatComponent.h"
#include "PlayerStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEPCCINE_API UPlayerStatComponent : public UBaseStatComponent
{
	GENERATED_BODY()

public:	
	UPlayerStatComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status|Player")
	float HealthDecelerationSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status|Player")
	float Stamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status|Player")
	float InvincibilityTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status|Player")
	float Defence;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status|Player")
	float FireRate;


protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
