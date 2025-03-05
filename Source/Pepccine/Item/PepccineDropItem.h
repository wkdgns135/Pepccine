#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PepccineDropItem.generated.h"

class UPepccineActiveItemData;
class UPepccineItemPriceWidget;
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
	void InitializeDropItem(UPepccineItemDataBase* InDropItemData, bool bInIsShopItem);

	// 아이템 위젯 on / off (show = true, hidden = false)
	UFUNCTION(BlueprintCallable, Category = "DropItem")
	void ShowInteractWidget(const bool bShow) const;

	// 아이템 획득
	UFUNCTION(BlueprintCallable)
	bool PickUpItem(UPepccineItemManagerComponent* ItemManagerComponent);

	// 무기 아이템 데이터 변경
	void ChangeWeaponItemData(UPepccineWeaponItemData* BeforeWeaponItemData);
	// 액티브 아이템 데이터 변경
	void ChangeActiveItemData(UPepccineActiveItemData* BeforeActiveItemData);

	// getter
	// 스태틱 메시 컴포넌트 가져오기
	FORCEINLINE UStaticMeshComponent* GetStaticMeshComp() const { return StaticMeshComp; };
	// 상호작용 위젯 컴포넌트 가져오기
	FORCEINLINE UWidgetComponent* GetInteractWidgetComp() const { return InteractWidgetComp; };
	// 가격 위젯 컴포넌트 가져오기
	FORCEINLINE UWidgetComponent* GetPriceWidgetComp() const { return PriceWidgetComp; };
	// 가격 위젯 가져오기
	FORCEINLINE UPepccineItemPriceWidget* GetPriceWidget() const { return PriceWidget; };
	// 아이템 데이터 가져오기
	FORCEINLINE UPepccineItemDataBase* GetDropItemData() const { return DropItemData; };

private:
	// 오버랩 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "DropItem|Component",
		meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereComp;
	// 스태틱 메시 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "DropItem|Component",
		meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComp;
	// 상호작용 위젯 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "DropItem|Component",
		meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* InteractWidgetComp;
	// 가격 위젯 컴포넌트(상점에서만 보여짐)
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "DropItem|Component",
		meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* PriceWidgetComp;
	// 가격 위젯
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "DropItem|Widget",
		meta = (AllowPrivateAccess = "true"))
	UPepccineItemPriceWidget* PriceWidget;
	// 드랍 아이템 데이터
	UPROPERTY(VisibleAnywhere, Category = "DropItem|Data")
	UPepccineItemDataBase* DropItemData;
	// 드랍 아이템 현재 탄약 수
	float MagazineAmmo;
	// 드랍 아이템 예비 탄약 수
	float SpareAmmo;

	// 상점 아이템 여부
	UPROPERTY(VisibleAnywhere, Category = "DropItem|Data")
	bool bIsShopItem = false;
	// 상점 아이템 가격
	int32 ItemPrice = 0;
	
	// 상하 이동 액션에 사용
	// 시작 위치
	FVector StartLocation;
	// 경과 시간
	float CurrentTime = 0.0f;
};
