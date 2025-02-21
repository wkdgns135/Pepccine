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
	// 스탯 적용


	
	// getter
	FORCEINLINE FPepccineWeaponStatModifier GetModifyWeaponStats() const { return ModifyWeaponStats; };

protected:
	// 무기 스탯 수정 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Passive", meta = (DisplayName = "무기 스탯 수정 목록"))
	FPepccineWeaponStatModifier ModifyWeaponStats;
};
