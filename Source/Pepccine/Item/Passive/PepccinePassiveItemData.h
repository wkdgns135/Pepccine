#pragma once

#include "CoreMinimal.h"
#include "Item/PepccineItemDataBase.h"
#include "Item/Weapon/PepccineWeaponStat.h"

#include "PepccinePassiveItemData.generated.h"

UCLASS(BlueprintType)
class PEPCCINE_API UPepccinePassiveItemData : public UPepccineItemDataBase
{
	GENERATED_BODY()
	
public:

	// 무기 스텟 수정 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Passive", meta = (DisplayName = "무기 스텟 수정 목록"))
	TMap<EPepccineWeaponStatName, float> ModifyWeaponStats;

	// 캐릭터 스텟 수정 목록
	//TMap<type, float> ModifyCharacterStats;

	// 캐릭터 기능 추가 목록


	// 패시브 발동
	virtual void UseItem(class UPepccineItemManagerComponent* ItemManagerComp) override;
};
