#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PepccineDropItem.generated.h"

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
	void InitializeDropItem(const UPepccineItemDataBase* InDropItemData);

	// 아이템 위젯 on / off (show = true, hidden = false)
	void ShowInteractWidget(const bool bShow) const;

	// 아이템 데이터 가져오기
	UPepccineItemDataBase* GetDropItemData() const { return DropItemData; };

	// 아이템 획득
	UFUNCTION(BlueprintCallable)
	void PickUpItem(UPepccineItemManagerComponent* ItemManagerComponent);

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
};
