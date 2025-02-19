#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAssetBase.generated.h"

class UWeaponItemData;

USTRUCT(BlueprintType)
struct FTestStruct
{
	GENERATED_BODY()

public:
	// 화면에 보여질 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (DisplayName = "이름"))
	FString DisplayName;
};

UCLASS()
class PEPCCINE_API UTestClass : public UObject
{
	GENERATED_BODY()

public:
	// 화면에 보여질 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (DisplayName = "이름"))
	FString DisplayName;
};

UCLASS(DefaultToInstanced, EditInlineNew)
class PEPCCINE_API UTestInstancedClass : public UObject
{
	GENERATED_BODY()

public:
	// 화면에 보여질 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (DisplayName = "이름"))
	FString DisplayName;
};

UCLASS(BlueprintType)
class PEPCCINE_API UItemDataAssetBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// 전체 무기 데이터 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UWeaponItemData>> Weapons;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FTestStruct> TestStruct;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UTestClass>> TestClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UTestInstancedClass>> TestInstancedClass;

	// TODO[명관] : UseableItemData 추가

};
