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
        return FString::Printf(TEXT("Health Stats\nCurrent: %.2f\nMax: %.2f\nDeceleration Speed: %.2f\nDeceleration Amount: %.2f\n"),
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

    FString PrintStats() const
    {
        return FString::Printf(TEXT("Stamina Stats\nCurrent: %.2f\nMax: %.2f\nRecovery Rate: %.2f\nRecovery Time: %.2f\n"),
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
        return FString::Printf(TEXT("Combat Stats\nAttack Damage: %.2f\nInvincibility Time: %.2f\nDefence: %.2f\n"),
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

    FString PrintStats() const
    {
        return FString::Printf(TEXT("Movement Stats\nSpeed: %.2f\nSprint: %.2f\nCrouch: %.2f\nRoll Dist: %.2f\nJumpZ: %.2f\nRoll Time: %.2f\n"),
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

    FString PrintStats() const
    {
        return FString::Printf(TEXT("===== Player Stats =====\n%s%s%s%s=======================\n"),
                               *HealthStats.PrintStats(),
                               *StaminaStats.PrintStats(),
                               *CombatStats.PrintStats(),
                               *MovementStats.PrintStats());
    }
};