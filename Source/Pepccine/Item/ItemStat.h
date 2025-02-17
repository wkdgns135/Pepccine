#pragma once

#include "CoreMinimal.h"
#include "ItemStat.generated.h"

/** 아이템 스텟 이름 */
UENUM(BlueprintType)
enum class EItemStatName : uint8
{
	AttackMultiplier UMETA(DisplayName = "공격력 배율"),
	RangeMultiplier UMETA(DisplayName = "사거리 배율"),
	FireRateMultiplier UMETA(DisplayName = "연사 배율"),
	ZoomMultiplier UMETA(DisplayName = "확대 배율"),
	MagazineCapacity UMETA(DisplayName = "탄창"),
	BulletSpeed UMETA(DisplayName = "탄속"),
	ReloadSpeed UMETA(DisplayName = "재장전 속도"),
	ProjectileCount UMETA(DisplayName = "투사체 개수"),
	BulletSpread UMETA(DisplayName = "탄 퍼짐"),
	Recoil UMETA(DisplayName = "반동"),
	Weight UMETA(DisplayName = "무게")
};

/** 아이템 스텟 */
USTRUCT(BlueprintType)
struct FItemStat
{
	GENERATED_BODY()

public:
	TMap<EItemStatName, float> Stats;
};
