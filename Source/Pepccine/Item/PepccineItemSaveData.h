#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
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
struct FPepccineItemSaveDataStruct
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
	// 액티브 아이템 아이디
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DesplayName = "액티브 아이템 아이디"))
	int32 ActiveItemId;
	// 코인 개수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DesplayName = "코인 개수"))
	int32 CoinCount;

	FPepccineItemSaveDataStruct(): MainWeaponItemId(-1), SubWeaponItemId(-1),
	                         EquippedWeaponItemType(EPepccineWeaponItemType::EPWIT_Main), ActiveItemId(-1), CoinCount(0)
	{
	}
};

UCLASS(BlueprintType)
class PEPCCINE_API UPepccineItemSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FPepccineItemSaveDataStruct ItemSaveData;

	UPepccineItemSaveData();
};
