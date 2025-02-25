#pragma once

#include "CoreMinimal.h"
#include "PepccineCharacterFeature.h"
#include "PepccineStatModifier.h"
#include "Item/PepccineItemDataBase.h"

#include "PepccinePassiveItemData.generated.h"

UCLASS(BlueprintType)
class PEPCCINE_API UPepccinePassiveItemData : public UPepccineItemDataBase
{
	GENERATED_BODY()

public:

	// 캐릭터 기능 추가 여부 확인
	UFUNCTION(BlueprintCallable, category = "Modifier")
	bool HasCharacterFeaturesByName(const EPepccineCharacterFeatureName CharacterFeatureName);
	
	// getter
	UFUNCTION(BlueprintPure, category = "Modifier")
	FORCEINLINE int32 GetPassiveItemId() const { return PassiveItemId; };
	
	UFUNCTION(BlueprintPure, category = "Modifier")
	FORCEINLINE TArray<FPepccineWeaponStatModifier> GetWeaponStatModifiers() const { return WeaponStatModifiers; };
	UFUNCTION(BlueprintPure, category = "Modifier")
	FORCEINLINE TArray<FPepccineCharacterStatModifier> GetCharacterStatModifiers() const
	{
		return CharacterStatModifiers;
	};

	// setter
	FORCEINLINE void SetPassiveItemId(const int32 Id) { PassiveItemId = Id; };

protected:
	// 패시브 아이템 아이디
	int32 PassiveItemId;

	// 무기 스탯 수정자 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Passive", meta = (DisplayName = "무기 스탯 수정 목록"))
	TArray<FPepccineWeaponStatModifier> WeaponStatModifiers;

	// 캐릭터 스탯 수정자 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Passive", meta = (DisplayName = "캐릭터 스탯 수정 목록"))
	TArray<FPepccineCharacterStatModifier> CharacterStatModifiers;

	// 캐릭터 추가 기능 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Passive",
		meta = (DisplayName = "캐릭터 추가 기능 목록"))
	TArray<FPepccineCharacterFeature> CharacterFeatures;
};
