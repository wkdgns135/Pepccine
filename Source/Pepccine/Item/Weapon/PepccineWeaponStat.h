#pragma once

#include "CoreMinimal.h"
#include "PepccineStatModifyType.h"

#include "PepccineWeaponStat.generated.h"

// 무기 타입
UENUM(BlueprintType)
enum class EPepccineWeaponItemType : uint8
{
	EPWIT_Main UMETA(DisplayName = "주 무기"),
	EPWIT_Sub UMETA(DisplayName = "보조 무기")
};

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

// 무기 스탯 구조체
USTRUCT(BlueprintType)
struct FPepccineWeaponStat
{
	GENERATED_BODY()

	// 공격력 배율
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats",
		meta = (DisplayName = "공격력 배율", ClampMin = "0.1", ClampMax = "10.0"))
	float AttackMultiplier;
	// 사거리 배율
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats",
		meta = (DisplayName = "사거리 배율", ClampMin = "0.1", ClampMax = "10.0"))
	float RangeMultiplier;
	// 연사 배율
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats",
		meta = (DisplayName = "연사 배율", ClampMin = "0.1", ClampMax = "10.0"))
	float FireRateMultiplier;
	// 확대 배율
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats",
		meta = (DisplayName = "확대 배율", ClampMin = "1.0", ClampMax = "10.0"))
	float ZoomMultiplier;
	// 한 번에 탄창에 장전 할 수 있는 탄약 수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats",
		meta = (DisplayName = "탄창 용량", ClampMin = "1", ClampMax = "999"))
	float MagazineSize;
	// 현재 탄창에 남아 있는 탄약 수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats",
		meta = (DisplayName = "현재 탄약 수", ClampMin = "0", ClampMax = "999"))
	float MagazineAmmo;
	// 현재 예비 탄약 수, 보조 무기는 무한
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats",
		meta = (DisplayName = "예비 탄약 수", ClampMin = "0", ClampMax = "999"))
	float SpareAmmo;
	// 탄속
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats",
		meta = (DisplayName = "탄속", ClampMin = "1.0", ClampMax = "999.0"))
	float BulletSpeed;
	// 재장전 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats",
		meta = (DisplayName = "재장전 속도", ClampMin = "0.1", ClampMax = "3.0"))
	float ReloadSpeed;
	// 투사체 개수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats",
		meta = (DisplayName = "투사체 개수", ClampMin = "1", ClampMax = "100"))
	float ProjectileCount;
	// 탄 퍼짐
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats",
		meta = (DisplayName = "탄 퍼짐", ClampMin = "0.0", ClampMax = "10.0"))
	float BulletSpread;
	// 반동
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats",
		meta = (DisplayName = "반동", ClampMin = "0.0", ClampMax = "10.0"))
	float Recoil;
	// 무게
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats",
		meta = (DisplayName = "무게", ClampMin = "0.1", ClampMax = "10.0"))
	float Weight;

	FPepccineWeaponStat(): AttackMultiplier(1.0f), RangeMultiplier(1.0f), FireRateMultiplier(1.0f),
	                       ZoomMultiplier(1.0f),
	                       MagazineSize(12.0f),
	                       MagazineAmmo(12.0f), SpareAmmo(48.0f),
	                       BulletSpeed(1.0f),
	                       ReloadSpeed(1.0f),
	                       ProjectileCount(1.0f),
	                       BulletSpread(0.0f), Recoil(0.0f),
	                       Weight(0.0f)
	{
	}
};

// 무기 수정자 구조체
USTRUCT(BlueprintType)
struct FPepccineWeaponStatModifier
{
	GENERATED_BODY()

	// 수정자 아이디
	int32 Id;

	// 연산 타입
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modifier",
		meta = (DisplayName = "연산 타입"))
	EPepccineStatModifyType StatModifyType;
	// 무기 타입
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modifier",
		meta = (DisplayName = "무기 타입"))
	EPepccineWeaponItemType WeaponItemType;
	// 무기 스탯 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modifier",
		meta = (DisplayName = "무기 스탯 이름"))
	EPepccineWeaponStatName WeaponItemStatName;
	// 수정 값
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modifier",
		meta = (DisplayName = "수정 값"))
	float StatModifyValue;

	// 기본 생성자
	FPepccineWeaponStatModifier(): Id(0), StatModifyType(EPepccineStatModifyType::EPSMT_Add),
	                               WeaponItemType(EPepccineWeaponItemType::EPWIT_Main),
	                               WeaponItemStatName(EPepccineWeaponStatName::EPWSN_AttackMultiplier),
	                               StatModifyValue(0.0f)
	{
	}
};
