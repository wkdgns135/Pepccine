#pragma once

#include "CoreMinimal.h"
#include "Weapon/PepccineWeaponStat.h"

#include "PepccineItemSaveData.generated.h"

class UPepccinePassiveItemData;
class UPepccineWeaponItemData;

USTRUCT(BlueprintType)
struct FPepccineSaveWeaponAmmo
{
	GENERATED_BODY()

	// 현재 탄약 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DesplayName = "현재 탄약 수"))
	float MagazinesAmmo;
	// 예비 탄약 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DesplayName = "예비 탄약 수"))
	float SpareAmmo;

	FPepccineSaveWeaponAmmo(): MagazinesAmmo(0.0f), SpareAmmo(0.0f)
	{
	}
};

USTRUCT(BlueprintType)
struct FPepccineItemSaveData
{
	GENERATED_BODY()

	// 원본 주 무기 아이디
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DesplayName = "주 무기 아이디"))
	int32 MainWeaponItemId;
	// 주 무기 현재 탄약 수 / 예비 탄약 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DesplayName = "주 무기 탄약"))
	FPepccineSaveWeaponAmmo MainWeaponAmmo;
	// 원본 보조 무기 아이디
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DesplayName = "보조 무기 아이디"))
	int32 SubWeaponItemId;
	// 보조 무기 현재 탄약 수 / 예비 탄약 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DesplayName = "보조 무기 탄약"))
	FPepccineSaveWeaponAmmo SubWeaponAmmo;
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

	FPepccineItemSaveData(const int32 InMainWeaponId, const FPepccineSaveWeaponAmmo InMainWeaponAmmo,const int32 InSubWeaponId, const FPepccineSaveWeaponAmmo InSubWeaponAmmo,
	                      const EPepccineWeaponItemType InEquippedWeaponItemType,
	                      const TArray<int32>& InPassiveItemIds)
	{
		MainWeaponItemId = InMainWeaponId;
		MainWeaponAmmo = InMainWeaponAmmo;
		SubWeaponItemId = InSubWeaponId;
		SubWeaponAmmo = InSubWeaponAmmo;
		EquippedWeaponItemType = InEquippedWeaponItemType;
		PassiveItemIds = InPassiveItemIds;
	}
};
