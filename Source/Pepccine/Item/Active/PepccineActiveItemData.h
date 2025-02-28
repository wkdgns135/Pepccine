#pragma once

#include "CoreMinimal.h"
#include "Item/PepccineItemDataBase.h"
#include "PepccineActiveItemData.generated.h"

struct FPepccineCharacterStatModifier;
struct FPepccineWeaponStatModifier;

UCLASS(BlueprintType, Abstract)
class PEPCCINE_API UPepccineActiveItemData : public UPepccineItemDataBase
{
	GENERATED_BODY()

public:
	// 액티브 효과 발동
	UFUNCTION(BlueprintCallable, Category = "Item|ActiveItem")
	virtual void ActivateItem()
	{
	}

protected:
	// 재사용 대기시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Active", meta = (DisplayName = "재사용 대기시간"))
	float Cooldown;
};

UCLASS(BlueprintType)
class PEPCCINE_API UPepccinePotionItemData : public UPepccineActiveItemData
{
	GENERATED_BODY()

public:
	virtual void ActivateItem() override;
	
	TArray<FPepccineWeaponStatModifier> GetWeaponStatModifiers() const;
	TArray<FPepccineCharacterStatModifier> GetCharacterStatModifiers() const;

protected:
	// 무기 스탯 수정 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Potion", meta = (DisplayName = "무기 스탯 수정 목록 / 지속시간"))
	TMap<FPepccineWeaponStatModifier, float> WeaponStatModifiers;

	// 캐릭터 스탯 수정 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Potion", meta = (DisplayName = "캐릭터 스탯 수정 목록 / 지속시간"))
	TMap<FPepccineCharacterStatModifier, float> CharacterStatModifiers;
};
