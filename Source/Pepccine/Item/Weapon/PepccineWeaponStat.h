#pragma once

#include "CoreMinimal.h"

#include "PepccineWeaponStat.generated.h"

// 무기 타입
UENUM(BlueprintType)
enum class EPepccineWeaponItemType : uint8
{
	EPWIT_Main UMETA(DisplayName = "주 무기"),
	EPWIT_Sub UMETA(DisplayName = "보조 무기")
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
	// 사거리(단위 m)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats",
		meta = (DisplayName = "사거리", ClampMin = "10.0", ClampMax = "1000.0"))
	float AttackRange;
	// 연사 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats",
		meta = (DisplayName = "연사 속도", ClampMin = "0.1", ClampMax = "100.0"))
	float FireRate;
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
	// 탄속(m/초)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats",
		meta = (DisplayName = "탄속", ClampMin = "10.0"))
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
		meta = (DisplayName = "탄 퍼짐", ClampMin = "1.0", ClampMax = "10.0"))
	float BulletSpread;
	// 반동
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats",
		meta = (DisplayName = "반동", ClampMin = "1.0", ClampMax = "10.0"))
	float Recoil;
	// 무게
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats",
		meta = (DisplayName = "무게", ClampMin = "0.1", ClampMax = "1000.0"))
	float Weight;

	FPepccineWeaponStat(): AttackMultiplier(1.0f), AttackRange(10.0f), FireRate(1.0f),
	                       ZoomMultiplier(1.0f),
	                       MagazineSize(12.0f),
	                       MagazineAmmo(12.0f), SpareAmmo(48.0f),
	                       BulletSpeed(10.0f),
	                       ReloadSpeed(1.0f),
	                       ProjectileCount(1.0f),
	                       BulletSpread(1.0f), Recoil(1.0f),
	                       Weight(1.0f)
	{
	}
};
