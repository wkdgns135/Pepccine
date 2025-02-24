#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PepccineDropItem.generated.h"

class UPepccineItemDataBase;
class USphereComponent;

UCLASS(Abstract)
class PEPCCINE_API APepccineDropItem : public AActor
{
	GENERATED_BODY()

public:
	APepccineDropItem();

	// 아이템 초기화
	void InitializeDropItem(const UPepccineItemDataBase* InDropItemData);
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	// 충돌 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, category = "DropItem|Component", meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereComp;
	// 스태틱 메시 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, category = "DropItem|Component", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComp;
	// 드랍 아이템 데이터
	UPROPERTY(VisibleAnywhere, category = "DropItem|Data")
	UPepccineItemDataBase* DropItemData;
};
