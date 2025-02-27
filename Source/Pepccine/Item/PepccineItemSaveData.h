#pragma once

#include "CoreMinimal.h"
#include "Weapon/PepccineWeaponStat.h"

#include "PepccineItemSaveData.generated.h"

class UPepccinePassiveItemData;
class UPepccineWeaponItemData;

USTRUCT(BlueprintType)
struct FPepccineItemSaveData
{
	GENERATED_BODY()

	// 원본 주 무기 아이디
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DesplayName = "주 무기 아이디"))
	int32 MainWeaponItemId;
	// 원본 보조 무기 아이디
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DesplayName = "보조 무기 아이디"))
	int32 SubWeaponItemId;
	// 장착 중인 무기 아이디
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DesplayName = "장착 무기 타입"))
	EPepccineWeaponItemType EquippedWeaponItemType;

	// 패시브 아이템 아이디 목록
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DesplayName = "패시브 아이템 아이디 목록"))
	TArray<int32> PassiveItemIds;

	FPepccineItemSaveData(): MainWeaponItemId(-1), SubWeaponItemId(-1),
	                         EquippedWeaponItemType(EPepccineWeaponItemType::EPWIT_Main)
	{
	}

	FPepccineItemSaveData(const int32 InMainWeaponId, const int32 InSubWeaponId,
	                      const EPepccineWeaponItemType InEquippedWeaponItemType,
	                      const TArray<int32>& InPassiveItemIds)
	{
		MainWeaponItemId = InMainWeaponId;
		SubWeaponItemId = InSubWeaponId;
		EquippedWeaponItemType = InEquippedWeaponItemType;
		PassiveItemIds = InPassiveItemIds;
	}
};
