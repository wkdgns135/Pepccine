// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseStatComponent.h"
#include "Character/Interfaces/IStaminaObserver.h"
#include "PlayerStatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEPCCINE_API UPlayerStatComponent : public UBaseStatComponent
{
	GENERATED_BODY()

public:	
	UPlayerStatComponent();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float HealthDecelerationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float HealthDecelerationAmount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float StaminaRecoveryRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float StaminaRecoveryTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float InvincibilityTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float Defence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float FireRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float SprintSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float CrouchSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float RollingDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float JumpZVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Player")
	float RollElapsedTime;

	void DecreaseHealth(float Amount);
	
	void DecreaseHealth_Timer();
	
	bool DecreaseStamina(float Amount);
	bool DecreaseStaminaByPercentage(float Percentage);

	void IncreaseStamina(float Amount);
	void IncreaseStaminaByPercentage(float Percentage);

	void AddStaminaObserver(IIStaminaObserver* Observer);
	void RemoveStaminaObserver(IIStaminaObserver* Observer);

	// inline
	FORCEINLINE_DEBUGGABLE float getCurrentStamina() const { return Stamina; }
	FORCEINLINE_DEBUGGABLE float getMaxStamina() const { return MaxStamina; }
	FORCEINLINE_DEBUGGABLE float getCurrentHealth() const { return CurrentHealth; }
	FORCEINLINE_DEBUGGABLE float getMaxHealth() const { return MaxHealth; }

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void NotifyStaminaObservers();
	void StartRepeatingTimer();

	TArray<IIStaminaObserver*> StaminaObservers;
	FTimerHandle IncreaseStaminaTimerHandle;
	FTimerHandle DecreaseHealthTimerHandle;
};
