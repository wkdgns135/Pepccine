#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/Weapon/PepccineWeaponItemData.h"

#include "PepccineItemManagerComponent.generated.h"

class UPepccineItemDataAssetBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PEPCCINE_API UPepccineItemManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPepccineItemManagerComponent();

	// getter
	FORCEINLINE TObjectPtr<UPepccineWeaponItemData> GetMainWeapon() const { return MainWeapon; };
	FORCEINLINE TObjectPtr<UPepccineWeaponItemData> GetSubWeapon() const { return SubWeapon; };
	FORCEINLINE TObjectPtr<UPepccineWeaponItemData> GetEquippedWeapon() const { return EquippedWeapon; };
	FORCEINLINE AActor* GetWeaponActor() const { return WeaponActor; };

protected:
	// 무기 액터 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<AActor> WeaponActorClass;

	// 무기 액터
	UPROPERTY(VisibleAnywhere, Category = "Item")
	AActor* WeaponActor;

	// 원본 데이터 에셋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UPepccineItemDataAssetBase* ItemDataAsset;

	// 주 무기
	UPROPERTY(VisibleAnywhere, Category = "Item", meta = (DisplayName = "주 무기"))
	TObjectPtr<UPepccineWeaponItemData> MainWeapon;
	// 보조 무기
	UPROPERTY(VisibleAnywhere, Category = "Item", meta = (DisplayName = "보조 무기"))
	TObjectPtr<UPepccineWeaponItemData> SubWeapon;
	// 현재 장착중인 무기
	UPROPERTY(VisibleAnywhere, Category = "Item", meta = (DisplayName = "현재 장착 중인 무기"))
	TObjectPtr<UPepccineWeaponItemData> EquippedWeapon;

	// 액티브 아이템

	// 카드키

	// 돈

	// 패시브 아이템 목록

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 기본 무기 초기화
	UFUNCTION()
	void InitializeWeapon();

	// 무기 장착
	UFUNCTION(BlueprintCallable, Category="Item|Weapon")
	void EquipWeapon(UPepccineWeaponItemData* Weapon);

	// 무기 교체
	UFUNCTION(BlueprintCallable, Category="Item|Weapon")
	void SwapWeapon(EPepccineWeaponItemType WeaponType);

	// 무기 메시 교체
	UFUNCTION()
	void ChangeWeaponEquippedMesh();

	// 현재 장착 중인 무기 발사
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")
	void FireWeapon();

	// 현재 장착 중인 무기 재장전
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")
	void ReloadWeapon() const;

private:
	// 캐릭터에 부착되어 있는 무기 액터 설정
	void SetWeaponActor();

};
