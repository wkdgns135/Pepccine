#pragma once

#include "CoreMinimal.h"
#include "PepccineWeaponItemComponent.h"
#include "PepccineWeaponStat.h"
#include "Item/PepccineItemSubManager.h"
#include "Item/PepccineStatName.h"
#include "UObject/Object.h"

#include "PepccineWeaponItemManager.generated.h"

class UPepccineItemSpawner;
class UPepccineWeaponItemData;

UCLASS()
class PEPCCINE_API UPepccineWeaponItemManager : public UPepccineItemSubManager
{
	GENERATED_BODY()

public:
	// 캐릭터에 부착되어 있는 무기 액터 설정
	void SetWeaponItemComponent(ACharacter* OwnerCharacter);
	
	// 기본 무기 장착
	void EquipDefaultWeapon(const UPepccineItemSpawner* ItemSpawner);
	// 무기 획득
	void PickUpItem(const UPepccineWeaponItemData* WeaponItemData);
	// 무기 장착
	void EquipWeapon(UPepccineWeaponItemData* Weapon, bool bIsPlayEquipSound = true) const;
	// 무기 메시 교체
	void ChangeWeaponEquippedMesh() const;
	// 무기 교체
	void SwapWeapon(EPepccineWeaponItemType WeaponType) const;
	// 현재 장착 중인 무기 발사
	void FireWeapon(float WeaponDamage) const;
	// 현재 장착 중인 무기 재장전
	void ReloadWeapon() const;

	// 무기 스탯 초기 설정
	void UpdateWeaponItemStats(EPepccineWeaponItemType WeaponItemType) const;
	
	// 스탯 계산
	float CalculateTotalValueFromDefault(EPepccineWeaponStatName WeaponItemStatName, float WeaponItemStat) const;

	// getter
	
	// 무기 컴포넌트 가져오기
	FORCEINLINE UPepccineWeaponItemComponent* GetWeaponItemComp() const { return WeaponItemComp; };
	// 무기 데이터 가져오기
	FORCEINLINE UPepccineWeaponItemData* GetWeaponItemData(
		const EPepccineWeaponItemType WeaponItemType)
	{
		return WeaponItemType == EPepccineWeaponItemType::EPWIT_Main
				   ? MainWeaponItemData
				   : SubWeaponItemData;
	}

	// 장착 중인 무기 데이터 가져오기
	FORCEINLINE UPepccineWeaponItemData* GetEquippedWeaponItemData() const
	{
		return WeaponItemComp->GetEquippedWeaponData();
	}

private:
	// 무기 컴포넌트
	UPROPERTY(VisibleInstanceOnly, Category = "Item|Weapon")
	UPepccineWeaponItemComponent* WeaponItemComp;

	// 원본 주 무기 데이터
	UPROPERTY(VisibleInstanceOnly, Category = "Item|Weapon", meta = (DisplayName = "원본 주 무기 데이터"))
	TObjectPtr<UPepccineWeaponItemData> MainWeaponItemData;
	// 원본 보조 무기 데이터
	UPROPERTY(VisibleInstanceOnly, Category = "Item|Weapon", meta = (DisplayName = "원본 보조 무기 데이터"))
	TObjectPtr<UPepccineWeaponItemData> SubWeaponItemData;
};
