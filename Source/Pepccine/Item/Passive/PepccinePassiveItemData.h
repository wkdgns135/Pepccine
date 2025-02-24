#pragma once

#include "CoreMinimal.h"
#include "Item/PepccineItemDataBase.h"
#include "Item/Weapon/PepccineWeaponStat.h"

#include "PepccinePassiveItemData.generated.h"

UCLASS(BlueprintType)
class PEPCCINE_API UPepccinePassiveItemData : public UPepccineItemDataBase
{
	GENERATED_BODY()

public:
	
	// getter
	FORCEINLINE int32 GetPassiveItemId() const { return PassiveItemId; };
	FORCEINLINE TArray<FPepccineWeaponStatModifier> GetModifyWeaponStats() const { return ModifyWeaponStats; };

	// setter
	FORCEINLINE void SetPassiveItemId(const int32 Id) { PassiveItemId = Id; };

protected:
	// 패시브 아이템 아이디
	int32 PassiveItemId;

	// 무기 스탯 수정 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Passive", meta = (DisplayName = "무기 스탯 수정 목록"))
	TArray<FPepccineWeaponStatModifier> ModifyWeaponStats;
};
