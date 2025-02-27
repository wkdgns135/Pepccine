#pragma once

#include "CoreMinimal.h"
#include "PepccineItemSpawner.h"
#include "PepccineStatName.h"
#include "Components/ActorComponent.h"
#include "Item/Weapon/PepccineWeaponItemData.h"
#include "Item/Weapon/PepccineWeaponItemComponent.h"

#include "PepccineItemManagerComponent.generated.h"

struct FPepccineItemSaveData;
class UPepccinePassiveItemData;
class UPepccineItemSpawner;
class UPepccineItemDataAssetBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PEPCCINE_API UPepccineItemManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPepccineItemManagerComponent();

	virtual void BeginPlay() override;

	float GetWeaponStatByName(EPepccineWeaponItemType WeaponType, EPepccineWeaponStatName WeaponStatName) const;
	
	// 아이템 획득
	void PickUpItem(UPepccineItemDataBase* DropItemData, bool bIsPlayPickUpSound = true);
	// 무기 획득
	void PickUpItem(const UPepccineWeaponItemData* WeaponItemData);
	// 패시브 획득
	void PickUpItem(const UPepccinePassiveItemData* PassiveItemData);

	// 패시브 아이템 제거
	void RemovePassiveItemDataById(const int32 ItemId);

	//////////////////////////////////////////////////////////////////////////
	// 데이터 저장 관련

	// 아이템 세이브 데이터 구조체 가져오기
	UFUNCTION(BlueprintCallable)
	FPepccineItemSaveData GetSaveItemData() const;

	// 아이템 세이브 데이터 불러오기
	UFUNCTION(BlueprintCallable)
	void LoadItemData(const FPepccineItemSaveData& SaveData);
	//////////////////////////////////////////////////////////////////////////
	

	// 기본 무기 초기화
	UFUNCTION()
	void EquipDefaultWeapon();

	// 무기 장착
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")
	void EquipWeapon(UPepccineWeaponItemData* Weapon, bool bIsPlayEquipSound = true) const;

	// 무기 교체
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")
	void SwapWeapon(EPepccineWeaponItemType WeaponType);

	// 무기 메시 교체
	UFUNCTION()
	void ChangeWeaponEquippedMesh() const;
	
	// 현재 장착 중인 무기 발사
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")
	void FireWeapon(float WeaponDamage) const;

	// 현재 장착 중인 무기 재장전
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")
	void ReloadWeapon() const;

	// 무기 스탯 합연산 및 곱연산 결과 값 가져오기
	UFUNCTION(BlueprintPure, Category = "Item|Weapon")
	float GetCalculatedWeaponItemStat(const EPepccineWeaponItemType WeaponItemType, const EPepccineWeaponStatName WeaponItemStatName);

	// 무기 스탯 업데이트
	// void UpdateWeaponStatByName(EPepccineWeaponStatName WeaponItemStatName);
	
	// getter

	// 아이템 스포너 가져오기
	UFUNCTION(BlueprintCallable, Category = "Item|Spawner")
	UPepccineItemSpawner* GetItemSpawner() const { return ItemSpawner; };

	// inline getter

	// 무기 컴포넌트 가져오기
	UFUNCTION(BlueprintPure, Category = "Item|Weapon")
	FORCEINLINE UPepccineWeaponItemComponent* GetWeaponItemComp() const { return WeaponItemComp; };
	// 무기 데이터 가져오기
	UFUNCTION(BlueprintPure, category = "Item|Weapon")
	FORCEINLINE UPepccineWeaponItemData* GetWeaponItemData(
		const EPepccineWeaponItemType WeaponItemType)
	{
		return WeaponItemType == EPepccineWeaponItemType::EPWIT_Main
			       ? MainWeaponItemData
			       : SubWeaponItemData;
	}

	// 장착 중인 무기 데이터 가져오기
	UFUNCTION(BlueprintPure, category = "Item|Weapon")
	FORCEINLINE UPepccineWeaponItemData* GetEquippedWeaponItemData() const
	{
		return WeaponItemComp->GetEquippedWeaponData();
	}

	// 현재 장착중인 무기가 메인 무기인지 확인
	FORCEINLINE bool IsMainWeaponEquipped() const
	{
		return GetEquippedWeaponItemData()->GetWeaponItemType() == EPepccineWeaponItemType::EPWIT_Sub;
	};

	// 전체 패시브 데이터 가져오기
	UFUNCTION(BlueprintPure, category = "Item|Passive")
	FORCEINLINE TMap<int32, UPepccinePassiveItemData*> GetPassiveItemDatas() { return PassiveItemDatas; };
	// 아이템 아이디로 패시브 데이터 가져오기
	UFUNCTION(BlueprintPure, category = "Item|Weapon")
	FORCEINLINE UPepccinePassiveItemData* GetPassiveItemById(const int32 ItemId) { return PassiveItemDatas[ItemId]; };
	// 아이템 데이터 가져오기
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE UPepccineItemDataAssetBase* GetItemData() const { return ItemSpawner->GetItemDataAsset(); };

	// 스탯 이름으로 무기 스탯 합연산 총합 가져오기
	FORCEINLINE float GetTotalSumByWeaponItemStatName(const EPepccineWeaponStatName WeaponItemStatName)
	{
		return TotalWeaponStatSum.FindOrAdd({GetEquippedWeaponItemData()->GetWeaponItemType(), WeaponItemStatName});
	};
	// 스탯 이름으로 무기 스탯 곱연산 총합 가져오기
	FORCEINLINE float GetTotalProductByWeaponItemStatName(const EPepccineWeaponStatName WeaponItemStatName)
	{
		return TotalWeaponStatProduct.FindOrAdd({GetEquippedWeaponItemData()->GetWeaponItemType(), WeaponItemStatName},
		                                        1.0f);
	};

	// 스탯 이름으로 캐릭터 스탯 합연산 총합 가져오기
	UFUNCTION(BlueprintPure, category = "Item")
	FORCEINLINE float GetTotalSumByCharacterStatName(const EPepccineCharacterStatName CharacterStatName)
	{
		return TotalCharacterStatSum.FindOrAdd(CharacterStatName);
	};
	// 스탯 이름으로 캐릭터 스탯 곱연산 총합 가져오기
	UFUNCTION(BlueprintPure, category = "Item")
	FORCEINLINE float GetTotalProductByCharacterStatName(const EPepccineCharacterStatName CharacterStatName)
	{
		return TotalCharacterStatProduct.FindOrAdd(CharacterStatName, 1.0f);
	};

private:
	// 아이템 스포너 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UPepccineItemSpawner> ItemSpawnerClass;

	// 임시 아이템 스포너
	UPROPERTY()
	UPepccineItemSpawner* ItemSpawner;

	// 무기 컴포넌트
	UPROPERTY(VisibleInstanceOnly, Category = "Item|Weapon")
	UPepccineWeaponItemComponent* WeaponItemComp;

	// 원본 주 무기 데이터
	UPROPERTY(VisibleInstanceOnly, Category = "Item|Weapon", meta = (DisplayName = "원본 주 무기 데이터"))
	TObjectPtr<UPepccineWeaponItemData> MainWeaponItemData;
	// 원본 보조 무기 데이터
	UPROPERTY(VisibleInstanceOnly, Category = "Item|Weapon", meta = (DisplayName = "원본 보조 무기 데이터"))
	TObjectPtr<UPepccineWeaponItemData> SubWeaponItemData;

	// 무기 스탯 합연산 총합
	TMap<TPair<EPepccineWeaponItemType, EPepccineWeaponStatName>, float> TotalWeaponStatSum;
	// 무기 스탯 곱연산 총합
	TMap<TPair<EPepccineWeaponItemType, EPepccineWeaponStatName>, float> TotalWeaponStatProduct;

	// 캐릭터 스탯 합연산 총합
	TMap<EPepccineCharacterStatName, float> TotalCharacterStatSum;
	// 캐릭터 스탯 곱연산 총합
	TMap<EPepccineCharacterStatName, float> TotalCharacterStatProduct;
	
	// 패시브 아이템 목록
	UPROPERTY(VisibleInstanceOnly, Category = "Item|Passive", meta = (DisplayName = "패시브 아이템 목록"))
	TMap<int32, UPepccinePassiveItemData*> PassiveItemDatas;

	// 액티브 아이템

	// 카드키

	// 돈

	// 캐릭터에 부착되어 있는 무기 액터 설정
	void SetWeaponItemComponent();
};
