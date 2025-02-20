#pragma once

#include "CoreMinimal.h"
#include "PepccineWeaponStat.generated.h"

// 무기 스텟 이름
UENUM(BlueprintType)
enum class EPepccineWeaponStatName : uint8
{
	EPWSN_AttackMultiplier UMETA(DisplayName = "공격력 배율"),
	EPWSN_RangeMultiplier UMETA(DisplayName = "사거리 배율"),
	EPWSN_FireRateMultiplier UMETA(DisplayName = "연사 배율"),
	EPWSN_ZoomMultiplier UMETA(DisplayName = "확대 배율"),
	EPWSN_MagazineSize UMETA(DisplayName = "탄창 용량"),
	EPWSN_BulletSpeed UMETA(DisplayName = "탄속"),
	EPWSN_ReloadSpeed UMETA(DisplayName = "재장전 속도"),
	EPWSN_ProjectileCount UMETA(DisplayName = "투사체 개수"),
	EPWSN_BulletSpread UMETA(DisplayName = "탄 퍼짐"),
	EPWSN_Recoil UMETA(DisplayName = "반동"),
	EPWSN_Weight UMETA(DisplayName = "무게")
};

// 무기 스텟 구조체
USTRUCT(BlueprintType)
struct FPepccineWeaponStat
{
	GENERATED_BODY()

	// 공격력 배율
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (DisplayName = "공격력 배율", ClampMin = "0.1", ClampMax = "10.0"));
	float AttackMultiplier = 1.0f;

	// 사거리 배율
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (DisplayName = "사거리 배율", ClampMin = "0.1", ClampMax = "10.0"));
	float RangeMultiplier = 1.0f;

	// 연사 배율
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (DisplayName = "연사 배율", ClampMin = "0.1", ClampMax = "10.0"));
	float FireRateMultiplier = 1.0f;

	// 확대 배율
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (DisplayName = "확대 배율", ClampMin = "0.1", ClampMax = "10.0"));
	float ZoomMultiplier = 1.0f;

	// 한 번에 탄창에 장전 할 수 있는 탄약 수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (DisplayName = "탄창 용량", ClampMin = "1", ClampMax = "999"));
	int32 MagazineSize = 12;

	// 현재 탄창에 남아 있는 탄약 수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (DisplayName = "현재 탄약 수", ClampMin = "0", ClampMax = "999"));
	int32 MagazineAmmo = 12;

	// 현재 예비 탄약 수, 보조 무기는 무한
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (DisplayName = "예비 탄약 수", ClampMin = "0", ClampMax = "999"));
	int32 SpareAmmo = 48;

	// 탄속
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (DisplayName = "탄속", ClampMin = "1.0", ClampMax = "999.0"));
	float BulletSpeed = 1.0f;
	
	// 재장전 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (DisplayName = "재장전 속도", ClampMin = "0.1", ClampMax = "3.0"));
	float ReloadSpeed = 1.0f;
	
	// 투사체 개수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (DisplayName = "투사체 개수", ClampMin = "1", ClampMax = "100"));
	int32 ProjectileCount = 1;
	
	// 반동
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (DisplayName = "반동", ClampMin = "0.0", ClampMax = "10.0"));
	float Recoil = 1.0f;
	
	// 무게
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (DisplayName = "무게", ClampMin = "0.1", ClampMax = "10.0"));
	float Weight = 1.0f;

};
