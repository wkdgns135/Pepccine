// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player/PlayerStatComponent.h"
#include "Character/Data/PlayerStatDataAsset.h"

UPlayerStatComponent::UPlayerStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();
	LoadAndApplyPlayerStatDataAsset();

	StartRepeatingTimer();
}

void UPlayerStatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(IncreaseStaminaTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(DecreaseHealthTimerHandle);
}

void UPlayerStatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerStatComponent::InitializeStats()
{
	if (PlayerStatDataAsset)
	{
		CurrentStats = PlayerStatDataAsset->DefaultStats;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerStatDataAsset이 설정되지 않았습니다! 기본값을 사용합니다."));

		CurrentStats.HealthStats.MaxHealth = 100.0f;
		CurrentStats.HealthStats.HealthDecelerationSpeed = 1.0f;
		CurrentStats.HealthStats.HealthDecelerationAmount = 1.0f;

		CurrentStats.StaminaStats.MaxStamina = 100.0f;
		CurrentStats.StaminaStats.StaminaRecoveryRate = 10.0f;
		CurrentStats.StaminaStats.StaminaRecoveryTime = 1.0f;

		CurrentStats.CombatStats.AttackDamage = 100.0f;
		CurrentStats.CombatStats.InvincibilityTime = 0.0f;
		CurrentStats.CombatStats.Defence = 0.0f;

		CurrentStats.MovementStats.MovementSpeed = 400.0f;
		CurrentStats.MovementStats.SprintSpeed = 800.0f;
		CurrentStats.MovementStats.CrouchSpeed = 200.0f;
		CurrentStats.MovementStats.RollingDistance = 200.0f;
		CurrentStats.MovementStats.JumpZVelocity = 500.0f;
		CurrentStats.MovementStats.RollElapsedTime = 0.0f;
	}
}

void UPlayerStatComponent::LoadAndApplyPlayerStatDataAsset()
{
	FString AssetPath = TEXT("/Game/Pepccine/Character/Player/Data/PlayerStatDataAsset.PlayerStatDataAsset");
	UPlayerStatDataAsset* LoadedStatAsset = Cast<UPlayerStatDataAsset>(
		StaticLoadObject(UPlayerStatDataAsset::StaticClass(), nullptr, *AssetPath));

	if (LoadedStatAsset)
	{
		SetPlayerStatDataAsset(LoadedStatAsset);
		UE_LOG(LogTemp, Log, TEXT("PlayerStatDataAsset이 성공적으로 로드 및 적용되었습니다!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerStatDataAsset 로드 실패! 경로를 확인하세요: %s"), *AssetPath);
	}
}

void UPlayerStatComponent::SetPlayerStatDataAsset(UPlayerStatDataAsset* NewDataAsset)
{
	if (!NewDataAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetPlayerStatDataAsset() 실패: 전달된 DataAsset이 NULL입니다!"));
		return;
	}

	PlayerStatDataAsset = NewDataAsset;
	InitializeStats();

	UE_LOG(LogTemp, Log, TEXT("새로운 PlayerStatDataAsset이 적용되었습니다."));
}

void UPlayerStatComponent::RecalculateStats()
{
	InitializeStats();

	for (const FStatModifier& Modifier : ActiveModifiers)
	{
		ApplySingleStatModifier(Modifier);
	}

	ClampStats();
}

void UPlayerStatComponent::ApplyStatModifier(const FStatModifier& Modifier)
{
	ActiveModifiers.Add(Modifier);
	ApplySingleStatModifier(Modifier);
	ClampStats();
}

void UPlayerStatComponent::RemoveStatModifier(const FStatModifier& Modifier)
{
	ActiveModifiers.RemoveAll([&](const FStatModifier& ExistingModifier)
	{
		return ExistingModifier == Modifier;
	});
	
	RecalculateStats();
}

void UPlayerStatComponent::ApplySingleStatModifier(const FStatModifier& Modifier)
{
    switch (Modifier.StatType)
    {
    case EPepccineCharacterStatName::EPCSN_CurrentHealth:
        CurrentStats.HealthStats.CurrentHealth += Modifier.AdditiveValue;
        CurrentStats.HealthStats.CurrentHealth *= Modifier.MultiplicativeValue;
        break;

    case EPepccineCharacterStatName::EPCSN_MaxHealth:
        CurrentStats.HealthStats.MaxHealth += Modifier.AdditiveValue;
        CurrentStats.HealthStats.MaxHealth *= Modifier.MultiplicativeValue;
        break;
    	
    case EPepccineCharacterStatName::EPCSN_HealthDecelerationSpeed:
        CurrentStats.HealthStats.HealthDecelerationSpeed += Modifier.AdditiveValue;
        CurrentStats.HealthStats.HealthDecelerationSpeed *= Modifier.MultiplicativeValue;
        break;

    case EPepccineCharacterStatName::EPCSN_Stamina:
        CurrentStats.StaminaStats.CurrentStamina += Modifier.AdditiveValue;
        CurrentStats.StaminaStats.CurrentStamina *= Modifier.MultiplicativeValue;
        break;

    case EPepccineCharacterStatName::EPCSN_MaxStamina:
        CurrentStats.StaminaStats.MaxStamina += Modifier.AdditiveValue;
        CurrentStats.StaminaStats.MaxStamina *= Modifier.MultiplicativeValue;
        break;

    case EPepccineCharacterStatName::EPCSN_StaminaRecoveryRate:
        CurrentStats.StaminaStats.StaminaRecoveryRate += Modifier.AdditiveValue;
        CurrentStats.StaminaStats.StaminaRecoveryRate *= Modifier.MultiplicativeValue;
        break;

    case EPepccineCharacterStatName::EPCSN_StaminaRecoveryTime:
        CurrentStats.StaminaStats.StaminaRecoveryTime += Modifier.AdditiveValue;
        CurrentStats.StaminaStats.StaminaRecoveryTime *= Modifier.MultiplicativeValue;
        break;

    case EPepccineCharacterStatName::EPCSN_AttackDamage:
    	CurrentStats.CombatStats.AttackDamage += Modifier.AdditiveValue;
    	CurrentStats.CombatStats.AttackDamage *= Modifier.MultiplicativeValue;
    	break;

    case EPepccineCharacterStatName::EPCSN_InvincibilityTime:
        CurrentStats.CombatStats.InvincibilityTime += Modifier.AdditiveValue;
        CurrentStats.CombatStats.InvincibilityTime *= Modifier.MultiplicativeValue;
        break;

    case EPepccineCharacterStatName::EPCSN_Defence:
        CurrentStats.CombatStats.Defence += Modifier.AdditiveValue;
        CurrentStats.CombatStats.Defence *= Modifier.MultiplicativeValue;
        break;

    case EPepccineCharacterStatName::EPCSN_MovementSpeed:
    	CurrentStats.MovementStats.MovementSpeed += Modifier.AdditiveValue;
    	CurrentStats.MovementStats.MovementSpeed *= Modifier.MultiplicativeValue;
    	break;

    case EPepccineCharacterStatName::EPCSN_SprintSpeed:
        CurrentStats.MovementStats.SprintSpeed += Modifier.AdditiveValue;
        CurrentStats.MovementStats.SprintSpeed *= Modifier.MultiplicativeValue;
        break;

    case EPepccineCharacterStatName::EPCSN_CrouchSpeed:
        CurrentStats.MovementStats.CrouchSpeed += Modifier.AdditiveValue;
        CurrentStats.MovementStats.CrouchSpeed *= Modifier.MultiplicativeValue;
        break;

    case EPepccineCharacterStatName::EPCSN_RollingDistance:
        CurrentStats.MovementStats.RollingDistance += Modifier.AdditiveValue;
        CurrentStats.MovementStats.RollingDistance *= Modifier.MultiplicativeValue;
        break;

    case EPepccineCharacterStatName::EPCSN_JumpZVelocity:
        CurrentStats.MovementStats.JumpZVelocity += Modifier.AdditiveValue;
        CurrentStats.MovementStats.JumpZVelocity *= Modifier.MultiplicativeValue;
        break;

    case EPepccineCharacterStatName::EPCSN_RollElapsedTime:
        CurrentStats.MovementStats.RollElapsedTime += Modifier.AdditiveValue;
        CurrentStats.MovementStats.RollElapsedTime *= Modifier.MultiplicativeValue;
        break;

    default:
        UE_LOG(LogTemp, Warning, TEXT("Unknown StatType in Modifier: %d"), static_cast<int32>(Modifier.StatType));
        break;
    }
}


void UPlayerStatComponent::ClampStats()
{
	CurrentStats.HealthStats.MaxHealth = FMath::Clamp(CurrentStats.HealthStats.MaxHealth, 50.0f, 1000.0f);
	CurrentStats.HealthStats.HealthDecelerationAmount = FMath::Clamp(CurrentStats.HealthStats.MaxHealth, 0.0f, 20.0f);
	CurrentStats.HealthStats.HealthDecelerationSpeed = FMath::Clamp(CurrentStats.HealthStats.MaxHealth, 0.0f, 1.0f);

	CurrentStats.StaminaStats.StaminaRecoveryTime = FMath::Clamp(CurrentStats.HealthStats.MaxHealth, 1.0f, 2.0f);
	CurrentStats.StaminaStats.MaxStamina = FMath::Clamp(CurrentStats.StaminaStats.MaxStamina, 50.0f, 500.0f);
	CurrentStats.StaminaStats.StaminaRecoveryRate = FMath::Clamp(CurrentStats.StaminaStats.StaminaRecoveryRate, 1.0f,100.0f);

	CurrentStats.CombatStats.InvincibilityTime = FMath::Clamp(CurrentStats.StaminaStats.StaminaRecoveryRate, 0.0f,100.0f);
	CurrentStats.CombatStats.AttackDamage = FMath::Clamp(CurrentStats.CombatStats.AttackDamage, 1.0f, 100000.0f);
	CurrentStats.CombatStats.Defence = FMath::Clamp(CurrentStats.CombatStats.Defence, 0.0f, 100.0f);

	CurrentStats.MovementStats.MovementSpeed = FMath::Clamp(CurrentStats.MovementStats.MovementSpeed, 200.0f, 800.0f);
	CurrentStats.MovementStats.SprintSpeed = FMath::Clamp(CurrentStats.MovementStats.SprintSpeed, 400.0f, 1500.0f);
	CurrentStats.MovementStats.CrouchSpeed = FMath::Clamp(CurrentStats.MovementStats.CrouchSpeed, 50.0f, 50.0f);
	CurrentStats.MovementStats.RollingDistance = FMath::Clamp(CurrentStats.MovementStats.RollingDistance, 100.0f, 400.0f);
	CurrentStats.MovementStats.JumpZVelocity = FMath::Clamp(CurrentStats.MovementStats.JumpZVelocity, 200.0f, 1500.0f);
	CurrentStats.MovementStats.RollElapsedTime = FMath::Clamp(CurrentStats.MovementStats.RollElapsedTime, 0.0f, 2.0f);
}

// Timer
#pragma region
void UPlayerStatComponent::StartRepeatingTimer()
{
	GetWorld()->GetTimerManager().SetTimer(IncreaseStaminaTimerHandle,
	                                       this,
	                                       &UPlayerStatComponent::IncreaseStamina_Timer,
	                                       CurrentStats.StaminaStats.StaminaRecoveryTime,
	                                       true);

	GetWorld()->GetTimerManager().SetTimer(DecreaseHealthTimerHandle,
	                                       this,
	                                       &UPlayerStatComponent::DecreaseHealth_Timer,
	                                       CurrentStats.HealthStats.HealthDecelerationSpeed,
	                                       true);
}

void UPlayerStatComponent::IncreaseStamina_Timer()
{
	IncreaseStamina(CurrentStats.StaminaStats.StaminaRecoveryRate);
}

void UPlayerStatComponent::DecreaseHealth_Timer()
{
	DecreaseHealth(CurrentStats.HealthStats.HealthDecelerationAmount);
}
#pragma endregion

// Health
#pragma region
void UPlayerStatComponent::DecreaseHealth(float Amount)
{
	if (Amount <= 0.0f || CurrentStats.HealthStats.CurrentHealth <= 0)
	{
		return;
	}

	CurrentStats.HealthStats.CurrentHealth = FMath::Clamp(CurrentStats.HealthStats.CurrentHealth - Amount, 0.0f, CurrentStats.HealthStats.MaxHealth);

	OnHealthChanged.Broadcast(CurrentStats.HealthStats.CurrentHealth, CurrentStats.HealthStats.MaxHealth);
}
#pragma endregion

// Stamina
#pragma region
void UPlayerStatComponent::IncreaseStamina(float Amount)
{
	if (Amount <= 0.0f || CurrentStats.StaminaStats.CurrentStamina >= CurrentStats.StaminaStats.MaxStamina)
	{
		return;
	}

	CurrentStats.StaminaStats.CurrentStamina = FMath::Clamp(CurrentStats.StaminaStats.CurrentStamina + Amount, 0.0f, CurrentStats.StaminaStats.MaxStamina);

	NotifyStaminaObservers();
}

void UPlayerStatComponent::IncreaseStaminaByPercentage(float Percentage)
{
	if (Percentage <= 0.0f || Percentage > 100.0f)
	{
		return;
	}

	float Amount = (CurrentStats.StaminaStats.MaxStamina * (Percentage / 100.0f));
	IncreaseStamina(Amount);
}

bool UPlayerStatComponent::DecreaseStamina(float Amount)
{
	if (Amount <= 0.0f || CurrentStats.StaminaStats.CurrentStamina < Amount)
	{
		return false;
	}

	CurrentStats.StaminaStats.CurrentStamina = FMath::Clamp(CurrentStats.StaminaStats.CurrentStamina - Amount, 0.0f, CurrentStats.StaminaStats.MaxStamina);

	NotifyStaminaObservers();

	return true;
}

bool UPlayerStatComponent::DecreaseStaminaByPercentage(float Percentage)
{
	if (Percentage <= 0.0f || Percentage > 100.0f)
	{
		return false;
	}

	float Amount = (CurrentStats.StaminaStats.MaxStamina * (Percentage / 100.0f));

	return DecreaseStamina(Amount);
}
#pragma endregion

// Observers
#pragma region
void UPlayerStatComponent::AddStaminaObserver(IIStaminaObserver* Observer)
{
	if (Observer && !StaminaObservers.Contains(Observer))
	{
		StaminaObservers.Add(Observer);
	}
}

void UPlayerStatComponent::RemoveStaminaObserver(IIStaminaObserver* Observer)
{
	if (Observer)
	{
		StaminaObservers.Remove(Observer);
	}
}

void UPlayerStatComponent::NotifyStaminaObservers()
{
	for (IIStaminaObserver* Observer : StaminaObservers)
	{
		if (Observer)
		{
			Observer->OnStaminaChanged(CurrentStats.StaminaStats.CurrentStamina, CurrentStats.StaminaStats.MaxStamina);
		}
	}
}
#pragma endregion
