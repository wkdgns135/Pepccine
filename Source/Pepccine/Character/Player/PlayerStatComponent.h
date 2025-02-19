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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float HealthDecelerationSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float Stamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float InvincibilityTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float Defence;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float FireRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status|Common")
	float SprintSpeed;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
