#pragma once

#include "CoreMinimal.h"
#include "PlayerStats.generated.h"

// 체력 관련 스탯
USTRUCT(BlueprintType)
struct FHealthStats
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
    float CurrentHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
    float MaxHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float HealthDecelerationSpeed = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float HealthDecelerationAmount = 0.0f;
    
    FString PrintStats() const
    {
        return FString::Printf(TEXT("현재 체력: %.2f\n최대 체력: %.2f\n"),
                               CurrentHealth, MaxHealth);
    }
};

// 스태미나 관련 스탯
USTRUCT(BlueprintType)
struct FStaminaStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
    float CurrentStamina = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
    float MaxStamina = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
    float StaminaRecoveryRate = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
    float StaminaRecoveryTime = 1.0f;

    FString PrintStats() const
    {
        return FString::Printf(TEXT("현재 스테미나: %.2f\n최대 스테미나: %.2f\n스테미나 회복량: %.2f\n스테미나 회복속도: %.2f\n"),
                               CurrentStamina, MaxStamina, StaminaRecoveryRate, StaminaRecoveryTime);
    }
};

// 전투 관련 스탯
USTRUCT(BlueprintType)
struct FCombatStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
    float AttackDamage = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float InvincibilityTime = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float Defence = 0.0f;

    FString PrintStats() const
    {
        return FString::Printf(TEXT("공격력: %.2f\n방어력: %.2f\n"),
                               AttackDamage, Defence);
    }
};

// 이동 관련 스탯
USTRUCT(BlueprintType)
struct FMovementStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stats")
    float MovementSpeed = 400.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintSpeed = 800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float CrouchSpeed = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float RollingDistance = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float JumpZVelocity = 500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float RollElapsedTime = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Strength = 300.0f;

    FString PrintStats() const
    {
        return FString::Printf(TEXT("이동속도: %.2f\n달리기 속도: %.2f\n점프력: %.2f\n힘: %.2f\n"),
                               MovementSpeed, SprintSpeed, JumpZVelocity, Strength);
    }
};

// 모든 스탯을 포함하는 구조체
USTRUCT(BlueprintType)
struct FPlayerStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FHealthStats HealthStats;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FStaminaStats StaminaStats;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FCombatStats CombatStats;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FMovementStats MovementStats;

    FString PrintStats() const
    {
        return FString::Printf(TEXT("%s%s%s%s\n"),
                               *HealthStats.PrintStats(),
                               *StaminaStats.PrintStats(),
                               *CombatStats.PrintStats(),
                               *MovementStats.PrintStats());
    }
};