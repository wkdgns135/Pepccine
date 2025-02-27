// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseStatComponent.h"
#include "Character/Data/PlayerStatDataAsset.h"
#include "Character/Data/PlayerStats.h"
#include "Character/Interfaces/IStaminaObserver.h"
#include "Character/Data/StatModifier.h"
#include "PlayerStatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatsUpdated, const FPlayerStats&, PlayerStats);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEPCCINE_API UPlayerStatComponent : public UBaseStatComponent
{
	GENERATED_BODY()

public:	
	UPlayerStatComponent();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChanged OnHealthChanged;

	void DecreaseHealth(float Amount);
	
	void DecreaseHealth_Timer();
	
	bool DecreaseStamina(float Amount);
	bool DecreaseStaminaByPercentage(float Percentage);

	void IncreaseStamina_Timer();

	void IncreaseStamina(float Amount);
	void IncreaseStaminaByPercentage(float Percentage);

	void AddStaminaObserver(IIStaminaObserver* Observer);
	void RemoveStaminaObserver(IIStaminaObserver* Observer);

	float GetAttackDamage() const { return AttackDamage; }

	// inline
	FORCEINLINE_DEBUGGABLE float getCurrentStamina() const { return CurrentStamina; }
	FORCEINLINE_DEBUGGABLE float getMaxStamina() const { return MaxStamina; }
	FORCEINLINE_DEBUGGABLE float getCurrentHealth() const { return CurrentHealth; }
	FORCEINLINE_DEBUGGABLE float getMaxHealth() const { return MaxHealth; }

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:
	float MaxHealth;
	float MovementSpeed;
	float AttackDamage;
	
	float HealthDecelerationSpeed;
	float HealthDecelerationAmount;
	
	float MaxStamina;
	float StaminaRecoveryRate;
	float StaminaRecoveryTime;
	
	float InvincibilityTime;
	float Defence;
	float FireRate;
	float SprintSpeed;
	float CrouchSpeed;
	float RollingDistance;
	float JumpZVelocity;
	float RollElapsedTime;
	
	void NotifyStaminaObservers();
	void StartRepeatingTimer();

	TArray<IIStaminaObserver*> StaminaObservers;
	
	FTimerHandle IncreaseStaminaTimerHandle;
	FTimerHandle DecreaseHealthTimerHandle;

	// 스텟 관련 리팩중
	/** 새로운 PlayerStatDataAsset을 설정하고 스탯을 초기화 */
	UFUNCTION(BlueprintCallable, Category="Stats")
	void SetPlayerStatDataAsset(UPlayerStatDataAsset* NewDataAsset);

	/* 스탯 불러오기 */
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void LoadAndApplyPlayerStatDataAsset();
	
	/** 현재 스탯 정보를 가져온다. */
	UFUNCTION(BlueprintCallable, Category = "Stats")
	const FPlayerStats& GetCurrentStats() const { return CurrentStats; }
	
	/** 모든 Modifier를 적용하여 스탯을 다시 계산 */
	void RecalculateStats();
	/** 특정 Modifier를 적용 */
	void ApplyStatModifier(const FStatModifier& Modifier);
	/** 특정 Modifier를 제거 */
	void RemoveStatModifier(const FStatModifier& Modifier);
	/** 특정 Modifier를 한 개만 적용 */
	void ApplySingleStatModifier(const FStatModifier& Modifier);
	/** 스탯 값이 제한을 초과하지 않도록 보정 */
	void ClampStats();
	/** 스탯 초기화 (기본값으로 리셋) */
	void InitializeStats();
	
	FPlayerStats CurrentStats;

	/** 적용된 스탯 수정 목록 (아이템, 버프 등) */
	TArray<FStatModifier> ActiveModifiers;

	/** 스탯 업데이트 델리게이트 (UI 업데이트 등에서 사용) */
	//UPROPERTY(BlueprintAssignable, Category = "Stats")
	//FOnStatsUpdated OnStatsUpdated;

private:
	/** 게임에서 사용할 기본 스탯을 저장하는 데이터 애셋 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	UPlayerStatDataAsset* PlayerStatDataAsset;
};
