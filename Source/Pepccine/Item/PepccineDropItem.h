#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PepccineDropItem.generated.h"

class UPepccineWeaponItemData;
class UPepccineItemManagerComponent;
class UWidgetComponent;
class UPepccineItemDataBase;
class USphereComponent;

UCLASS(Abstract)
class PEPCCINE_API APepccineDropItem : public AActor
{
	GENERATED_BODY()

public:
	APepccineDropItem();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	// 아이템 초기화
	void InitializeDropItem(const UPepccineItemDataBase* InDropItemData, bool bInIsShopItem);

	// 아이템 위젯 on / off (show = true, hidden = false)
	UFUNCTION(BlueprintCallable, category = "DropItem")
	void ShowInteractWidget(const bool bShow) const;

	// 아이템 획득
	UFUNCTION(BlueprintCallable)
	void PickUpItem(UPepccineItemManagerComponent* ItemManagerComponent);

	// 무기 데이터 변경
	void ChangeWeaponItemData(UPepccineWeaponItemData* WeaponItemData) const;

	// getter
	// 스태틱 메시 컴포넌트 가져오기
	FORCEINLINE UStaticMeshComponent* GetStaticMeshComp() const { return StaticMeshComp; };
	// 위젯 컴포넌트 가져오기
	FORCEINLINE UWidgetComponent* GetInteractWidgetComp() const { return InteractWidgetComp; };
	// 아이템 데이터 가져오기
	FORCEINLINE UPepccineItemDataBase* GetDropItemData() const { return DropItemData; };

private:
	// 오버랩 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, category = "DropItem|Component",
		meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereComp;
	// 스태틱 메시 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, category = "DropItem|Component",
		meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComp;
	// 상호 작용 위젯 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, category = "DropItem|Component",
		meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* InteractWidgetComp;
	// 드랍 아이템 데이터
	UPROPERTY(VisibleAnywhere, category = "DropItem|Data")
	UPepccineItemDataBase* DropItemData;

	// 상점 아이템 여부
	bool bIsShopItem = false;
	
	// 상하 이동 액션에 사용
	// 시작 위치
	FVector StartLocation;
	// 경과 시간
	float CurrentTime = 0.0f;
};
