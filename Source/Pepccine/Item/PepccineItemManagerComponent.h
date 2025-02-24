#pragma once

#include "CoreMinimal.h"
#include "PepccineItemSpawner.h"
#include "Components/ActorComponent.h"
#include "Item/Weapon/PepccineWeaponItemData.h"
#include "Item/Weapon/PepccineWeaponItemComponent.h"

#include "PepccineItemManagerComponent.generated.h"

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

	// int 값처럼 사용하기 위한 소수점 아래 버림
	static FORCEINLINE void TruncateFloatStat(float& FloatStat) { FloatStat = FMath::TruncToFloat(FloatStat); };

	// 이름으로 무기 스탯 찾기
	static float& GetWeaponItemStatByName(FPepccineWeaponStat* WeaponItemStats, const EPepccineWeaponStatName StatName);

	// 무기 획득
	TObjectPtr<UPepccineWeaponItemData> PickUpWeaponItem(const UPepccineWeaponItemData* WeaponItemData);

	// 패시브 아이템 추가
	void AddPassiveItemData(const UPepccinePassiveItemData* PassiveItemData);
	// 패시브 아이템 제거
	void RemovePassiveItemDataById(const int32 Id);

	// getter

	// 아이템 스포너 가져오기
	UFUNCTION(BlueprintCallable, Category = "Item|Spawner")
	UPepccineItemSpawner* GetItemSpawner() const { return ItemSpawner; };
	// 수정본 무기 데이터 가져오기
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")

	// inline getter

	// 무기 컴포넌트 가져오기
	FORCEINLINE UPepccineWeaponItemComponent* GetWeaponItemComp() const { return WeaponItemComp; };
	// 무기 데이터 가져오기
	FORCEINLINE TObjectPtr<UPepccineWeaponItemData> GetWeaponItemData(
		const EPepccineWeaponItemType WeaponItemType)
	{
		return WeaponItemType == EPepccineWeaponItemType::EPWIT_Main
			       ? MainWeaponItemData
			       : SubWeaponItemData;
	}

	// 장착 중인 무기 데이터 가져오기
	UFUNCTION(BlueprintCallable, category = "Item|Weapon")
	FORCEINLINE UPepccineWeaponItemData* GetEquippedWeaponItemData() const
	{
		if (WeaponItemComp)
		{
			return WeaponItemComp->GetEquippedWeaponData();
		}
		return nullptr;
	}

	// 주 무기 데이터 가져오기
	UFUNCTION(BlueprintCallable, category = "Item|Weapon")
	FORCEINLINE UPepccineWeaponItemData* GetMainWeaponItemData() const { return MainWeaponItemData; };
	// 보조 무기 데이터 가져오기
	UFUNCTION(BlueprintCallable, category = "Item|Weapon")
	FORCEINLINE UPepccineWeaponItemData* GetSubWeaponItemData() const { return SubWeaponItemData; };
	// 전체 패시브 데이터 가져오기
	UFUNCTION(BlueprintCallable, category = "Item|Passive")
	FORCEINLINE TMap<int32, UPepccinePassiveItemData*> GetPassiveItemDatas() { return PassiveItemDatas; };
	// 번호로 패시브 데이터 가져오기
	UFUNCTION(BlueprintCallable, category = "Item|Weapon")
	FORCEINLINE UPepccinePassiveItemData* GetPassiveItemById(const int32 Id) { return PassiveItemDatas[Id]; };
	// 아이템 데이터 가져오기
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE UPepccineItemDataAssetBase* GetItemData() const { return ItemSpawner->GetItemDataAsset(); };

	// 스탯 이름으로 합연산 총합 가져오기
	FORCEINLINE float GetTotalSumByWeaponItemStatName(const EPepccineWeaponStatName WeaponItemStatName) const
	{
		return TotalSum[WeaponItemStatName];
	};
	// 스탯 이름으로 곱연산 총합 가져오기
	FORCEINLINE float GetTotalProductByWeaponItemStatName(const EPepccineWeaponStatName WeaponItemStatName) const
	{
		return TotalProduct[WeaponItemStatName];
	};

protected:
	// 기본 무기 초기화
	UFUNCTION()
	void EquipDefaultWeapon();

	// 무기 장착
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")
	void EquipWeapon(UPepccineWeaponItemData* Weapon);

	// 무기 교체
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")
	void SwapWeapon(EPepccineWeaponItemType WeaponType);

	// 무기 메시 교체
	UFUNCTION()
	void ChangeWeaponEquippedMesh() const;

	// 현재 장착 중인 무기 발사
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")
	void FireWeapon() const;

	// 현재 장착 중인 무기 재장전
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")
	void ReloadWeapon() const;

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

	// 합연산 총합
	TMap<EPepccineWeaponStatName, float> TotalSum;
	// 곱연산 총합
	TMap<EPepccineWeaponStatName, float> TotalProduct;

	// 패시브 아이템 아이디
	int32 PassiveItemId = 0;

	// 패시브 아이템 목록
	UPROPERTY(VisibleInstanceOnly, Category = "Item|Passive", meta = (DisplayName = "패시브 아이템 목록"))
	TMap<int32, UPepccinePassiveItemData*> PassiveItemDatas;

	// 액티브 아이템

	// 카드키

	// 돈

	// 캐릭터에 부착되어 있는 무기 액터 설정
	void SetWeaponItemComponent();
};
