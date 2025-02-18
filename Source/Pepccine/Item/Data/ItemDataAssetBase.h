#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAssetBase.generated.h"

USTRUCT(BlueprintType)
struct FItemDataInfo 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (MultiLine = "true"))
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	int32 SellingPrice;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	TObjectPtr<UStaticMesh> SpawnMesh;
};

USTRUCT(BlueprintType)
struct FWeaponItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FItemDataInfo Info;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USkeletalMesh> EquippedMesh;

	// TODO[명관] : 무기 스텟 업데이트

	// 임시 스텟

	// 공격력 배율
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Stats", meta = (DisplayName = "공격력 배율"));
	float AttackMultiplier = 1.0f;
	// 최대 탄창 용량
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Stats", meta = (DisplayName = "최대 탄창 용량"));
	int32 MagazineCapacity = 30;
};

USTRUCT(BlueprintType)
struct FPassiveItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Passive");
	FItemDataInfo Info;

	// TODO[명관] : 기능 추가
};


UCLASS()
class PEPCCINE_API UItemDataAssetBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FWeaponItemData> Weapons;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FPassiveItemData> Passives;

	// TODO[명관] : UseableItemData 추가

};
