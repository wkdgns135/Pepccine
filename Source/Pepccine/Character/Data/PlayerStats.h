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
    
    void PrintStats() const
    {
        UE_LOG(LogTemp, Log, TEXT("Health Stats -> Current: %.2f, Max: %.2f, Deceleration Speed: %.2f, Deceleration Amount: %.2f"),
            CurrentHealth, MaxHealth, HealthDecelerationSpeed, HealthDecelerationAmount);
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

    void PrintStats() const
    {
        UE_LOG(LogTemp, Log, TEXT("Stamina Stats -> Current: %.2f, Max: %.2f, Recovery Rate: %.2f, Recovery Time: %.2f"),
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

    void PrintStats() const
    {
        UE_LOG(LogTemp, Log, TEXT("Combat Stats -> Attack Damage: %.2f, Invincibility Time: %.2f, Defence: %.2f"),
            AttackDamage, InvincibilityTime, Defence);
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

    void PrintStats() const
    {
        UE_LOG(LogTemp, Log, TEXT("Movement Stats -> Speed: %.2f, Sprint: %.2f, Crouch: %.2f, Roll Dist: %.2f, JumpZ: %.2f, Roll Time: %.2f"),
            MovementSpeed, SprintSpeed, CrouchSpeed, RollingDistance, JumpZVelocity, RollElapsedTime);
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

    void PrintStats() const
    {
        UE_LOG(LogTemp, Log, TEXT("===== Player Stats ====="));
        HealthStats.PrintStats();
        StaminaStats.PrintStats();
        CombatStats.PrintStats();
        MovementStats.PrintStats();
        UE_LOG(LogTemp, Log, TEXT("======================="));
    }
};