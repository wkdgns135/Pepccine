#pragma once

#include "CoreMinimal.h"
#include "Item/Weapon/PepccineWeaponStat.h"

#include "WeaponStatModifier.generated.h"

class UPepccineWeaponItemData;

UCLASS()
class PEPCCINE_API UWeaponStatModifier : public UObject
{
	GENERATED_BODY()

public:
	// 이름으로 무기 스탯 찾기
	static float& GetWeaponItemStatByName(FPepccineWeaponStat* WeaponItemStats, EPepccineWeaponStatName StatName);
	// 무기 스탯 설정
	void SetWeaponItemStats(const UPepccineWeaponItemData* WeaponItemData);

	// 무기 스탯 수정자 추가
	void AddWeaponStatModifier(FPepccineWeaponStatModifier WeaponStatModifier);
	// 수정자 제거
	void RemoveWeaponStatModifier(const FPepccineWeaponStatModifier& WeaponStatModifier);

	// 무기 타입에 맞는 전체 스탯 수정자 적용
	void ApplyAllStatModifiers(EPepccineWeaponItemType WeaponItemType);

	// 무기 타입에 맞는 단일 스탯 수정자 적용
	void ApplyStatModifier(FPepccineWeaponStatModifier WeaponItemModifier, bool IsNewModifier);

	// getter
	FPepccineWeaponStat* GetEquippedWeaponItemStats(const EPepccineWeaponItemType WeaponItemType)
	{
		return WeaponItemType == EPepccineWeaponItemType::EPWIT_Main
			       ? &ModifiedMainWeaponItemStats
			       : &ModifiedSubWeaponItemStats;
	}

	FORCEINLINE FPepccineWeaponStat GetModifiedMainWeaponItemStats() const { return ModifiedMainWeaponItemStats; };
	FORCEINLINE FPepccineWeaponStat GetModifiedSubWeaponItemStats() const { return ModifiedSubWeaponItemStats; };

private:
	// 수정된 주 무기 스탯
	FPepccineWeaponStat ModifiedMainWeaponItemStats;
	// 수정된 보조 무기 스탯
	FPepccineWeaponStat ModifiedSubWeaponItemStats;

	// 무기 스탯 수정자 아이디
	int32 WeaponStatModifierId = 0;

	// 현재 적용되어 있는 무기 스탯 수정자(Key = 수정할 스탯 이름, Value = 적용 되어있는 수정자 목록)
	TMap<EPepccineWeaponStatName, TMap<int32, FPepccineWeaponStatModifier>> AppliedMainWeaponItemStatModifiers; // 주무기
	TMap<EPepccineWeaponStatName, TMap<int32, FPepccineWeaponStatModifier>> AppliedSubWeaponItemStatModifiers; // 보조 무기
};
