#pragma once

#include "CoreMinimal.h"

// 무기 스탯 이름
UENUM(BlueprintType)
enum class EPepccineWeaponStatName : uint8
{
	EPWSN_AttackMultiplier UMETA(DisplayName = "공격력 배율"),
	EPWSN_RangeMultiplier UMETA(DisplayName = "사거리 배율"),
	EPWSN_FireRateMultiplier UMETA(DisplayName = "연사 배율"),
	EPWSN_ZoomMultiplier UMETA(DisplayName = "확대 배율"),
	EPWSN_MagazineSize UMETA(DisplayName = "탄창 용량"),
	EPWSN_MagazineAmmo UMETA(DisplayName = "현재 탄약 수"),
	EPWSN_SpareAmmo UMETA(DisplayName = "예비 탄약 수"),
	EPWSN_BulletSpeed UMETA(DisplayName = "탄속"),
	EPWSN_ReloadSpeed UMETA(DisplayName = "재장전 속도"),
	EPWSN_ProjectileCount UMETA(DisplayName = "투사체 개수"),
	EPWSN_BulletSpread UMETA(DisplayName = "탄 퍼짐"),
	EPWSN_Recoil UMETA(DisplayName = "반동"),
	EPWSN_Weight UMETA(DisplayName = "무게")
};

UENUM(BlueprintType)
enum class EPepccineCharacterStatName : uint8
{
	EPCSN_CurrentHealth UMETA(DisplayName = "현재 체력"),
	EPCSN_MaxHealth UMETA(DisplayName = "최대 체력"),
	EPCSN_MovementSpeed UMETA(DisplayName = "이동 속도"),
	EPCSN_AttackDamage UMETA(DisplayName = "공격력"),
	EPCSN_HealthDecelerationSpeed UMETA(DisplayName = "체력 감소 속도"),
	EPCSN_Stamina UMETA(DisplayName = "현재 기력"),
	EPCSN_MaxStamina UMETA(DisplayName = "최대 기력"),
	EPCSN_StaminaRecoveryRate UMETA(DisplayName = "기력 회복 속도"),
	EPCSN_StaminaRecoveryTime UMETA(DisplayName = "기력 회복 시간"),
	EPCSN_InvincibilityTime UMETA(DisplayName = "무적 시간"),
	EPCSN_Defence UMETA(DisplayName = "방어력"),
	EPCSN_FireRate UMETA(DisplayName = "무기 발사 속도"),
	EPCSN_SprintSpeed UMETA(DisplayName = "달리기 속도"),
	EPCSN_CrouchSpeed UMETA(DisplayName = "앉기 속도"),
	EPCSN_RollingDistance UMETA(DisplayName = "구르기 거리"),
	EPCSN_JumpZVelocity UMETA(DisplayName = "점프 속도"),
	EPCSN_RollElapsedTime UMETA(DisplayName = "구르기 시간")
};
