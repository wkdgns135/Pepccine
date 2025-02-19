#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponItemData.h"

#include "ItemManagerComponent.generated.h"

class UItemDataAssetBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PEPCCINE_API UItemManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UItemManagerComponent();

	FORCEINLINE TObjectPtr<UWeaponItemData> GetMainWeapon() const { return MainWeapon; };
	FORCEINLINE TObjectPtr<UWeaponItemData> GetSubWeapon() const { return SubWeapon; };
	FORCEINLINE TObjectPtr<UWeaponItemData> GetEquippedWeapon() const { return EquippedWeapon; };

protected:
	// 무기 액터 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Weapon")
	TSubclassOf<AActor> WeaponActorClass;

	// 무기 액터
	UPROPERTY(VisibleAnywhere, Category = "Item|Weapon")
	TObjectPtr<AActor> WeaponActor;

	// 원본 데이터 에셋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UItemDataAssetBase> DefaultItemDataAsset;

	// 복사 데이터 에셋(사용할 데이터 에셋)
	UPROPERTY()
	TObjectPtr<UItemDataAssetBase> ItemDataAsset;

	// 주 무기
	UPROPERTY(VisibleAnywhere, Category = "Item|Weapon", meta = (DisplayName = "주 무기"))
	TObjectPtr<UWeaponItemData> MainWeapon;
	// 보조 무기
	UPROPERTY(VisibleAnywhere, Category = "Item|Weapon", meta = (DisplayName = "보조 무기"))
	TObjectPtr<UWeaponItemData> SubWeapon;
	// 현재 장착중인 무기
	UPROPERTY(VisibleAnywhere, Category = "Item|Weapon", meta = (DisplayName = "현재 장착 중인 무기"))
	TObjectPtr<UWeaponItemData> EquippedWeapon;

	// 액티브 아이템

	// 카드키

	// 돈

	// 패시브 아이템 목록

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 기본 무기 초기화
	UFUNCTION()
	void InitializeWeapon();

	// 무기 스왑
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")
	void SwapWeapons(EWeaponItemType WeaponItemType);

	// 현재 장착 중인 무기 발사
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")
	void FireWeapon() const;

	// 현재 장착 중인 무기 재장전
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")
	void ReloadWeapon() const;

private:
	// 캐릭터에 부착되어 있는 무기 액터 설정
	void SetWeaponActor();

};
