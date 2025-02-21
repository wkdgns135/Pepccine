#pragma once

#include "CoreMinimal.h"
#include "PepccineStatModifyType.h"
#include "Item/Weapon/PepccineWeaponStat.h"

#include "WeaponStatModifier.generated.h"

class UPepccineWeaponItemData;

UCLASS()
class PEPCCINE_API UWeaponStatModifier : public UObject
{
	GENERATED_BODY()

public:
	// 합연산
	static float Additive(float& Stat, float Amount);
	// 곱연산
	static float Multiplicative(float& Stat, float Amount);

	// 이름으로 무기 스탯 찾기
	float& GetWeaponStatByName(EPepccineWeaponStatName StatName) const;

	// 무기 스탯 수정
	UFUNCTION()
	void ModifyStat(EPepccineStatModifyType StatModifyType, EPepccineWeaponStatName StatName, float Amount) const;
	// 무기 스탯 수정자 추가
	void AddWeaponStatModifier(FPepccineWeaponStatModifier WeaponStatModifier);;

	// setter
	FORCEINLINE void SetEquippedWeaponItemStats(FPepccineWeaponStat* WeaponStats)
	{
		EquippedWeaponItemStats = WeaponStats;
	};

	FORCEINLINE void RemoveWeaponStatModifierById(const int32 Id) { AppliedWeaponStatModifiers.Remove(Id); };

private:
	// 현재 장착된 무기 스탯
	FPepccineWeaponStat* EquippedWeaponItemStats;

	// 무기 스탯 수정자 아이디
	int32 WeaponStatModifierId = 0;
	
	// 현재 적용되어 있는 무기 스탯 수정자(Key = Id, Value = 수정자)
	TMap<int32, FPepccineWeaponStatModifier> AppliedWeaponStatModifiers;
};
