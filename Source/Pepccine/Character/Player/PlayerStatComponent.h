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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float HealthDecelerationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float InvincibilityTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float Defence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float FireRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Common")
	float SprintSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Common")
	float CrouchSpeed;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
