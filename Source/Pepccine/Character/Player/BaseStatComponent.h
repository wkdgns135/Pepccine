// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEPCCINE_API UBaseStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBaseStatComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status|Common")
	float CurrentHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status|Common")
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status|Common")
	float MovementSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status|Common")
	float AttackDamage;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
