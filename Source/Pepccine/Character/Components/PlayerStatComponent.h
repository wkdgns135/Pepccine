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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEPCCINE_API UPlayerStatComponent : public UBaseStatComponent
{
	GENERATED_BODY()

public:	
	UPlayerStatComponent();

	TArray<IIStaminaObserver*> StaminaObservers;
	
	FTimerHandle IncreaseStaminaTimerHandle;
	FTimerHandle DecreaseHealthTimerHandle;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChanged OnHealthChanged;

	void CheckHealthStatus() const;
	void DecreaseHealth(float Amount);
	
	void DecreaseHealth_Timer();
	
	bool DecreaseStamina(float Amount);
	bool DecreaseStaminaByPercentage(float Percentage);

	void IncreaseStamina_Timer();

	void IncreaseStamina(float Amount);
	void IncreaseStaminaByPercentage(float Percentage);

	void AddStaminaObserver(IIStaminaObserver* Observer);
	void RemoveStaminaObserver(IIStaminaObserver* Observer);
	void NotifyStaminaObservers();
	
	void StartRepeatingTimer();

	float GetAttackDamage() const { return CurrentStats.CombatStats.AttackDamage; }

	// inline
	FORCEINLINE_DEBUGGABLE float GetCurrentStamina() const { return CurrentStats.StaminaStats.CurrentStamina; }
	FORCEINLINE_DEBUGGABLE float GetMaxStamina() const { return CurrentStats.StaminaStats.MaxStamina; }
	FORCEINLINE_DEBUGGABLE float GetCurrentHealth() const { return CurrentStats.HealthStats.CurrentHealth; }
	FORCEINLINE_DEBUGGABLE float GetMaxHealth() const { return CurrentStats.HealthStats.MaxHealth; }
	FORCEINLINE_DEBUGGABLE FString PrintStats() const { return CurrentStats.PrintStats(); }

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:
	/** 새로운 PlayerStatDataAsset을 설정하고 스탯을 초기화 */
	UFUNCTION(BlueprintCallable, Category="Stats")
	void SetPlayerStatDataAsset(UPlayerStatDataAsset* NewDataAsset);

	/* 스탯 불러오기 */
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void LoadAndApplyPlayerStatDataAsset();
	
	/** 현재 스탯 정보를 가져온다. */
	UFUNCTION(BlueprintCallable, Category = "Stats")
	const FPlayerStats& GetCurrentStats() const { return CurrentStats; }

	/** 스탯 초기화 (기본값으로 리셋) */
	void InitializeStats();
	/** 특정 Modifier를 적용 */
	void ApplyStatModifier(const FStatModifier& Modifier);
	/** 특정 Modifier를 제거 */
	void RemoveStatModifier(const FStatModifier& Modifier);

	/** 적용된 스탯 수정 목록 (아이템, 버프 등) */
	TArray<FStatModifier> ActiveModifiers;
	/* 현재 스텟 */
	FPlayerStats CurrentStats;
	/** 현재 적용된 모든 Modifier의 합연산 총합 */
	FPlayerStats CurrentTotalAdd;
	/** 현재 적용된 모든 Modifier의 곱연산 총합 */
	FPlayerStats CurrentTotalMul;

private:
	/** 모든 Modifier를 적용하여 스탯을 다시 계산 */
	void RecalculateStats();
	/** 스탯 값이 제한을 초과하지 않도록 보정 */
	void ClampStats();
	
	/** 게임에서 사용할 기본 스탯을 저장하는 데이터 애셋 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	UPlayerStatDataAsset* PlayerStatDataAsset;
};
