#pragma once

#include "CoreMinimal.h"

#include "PepccineItemDataBase.generated.h"

UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class PEPCCINE_API UPepccineItemDataBase : public UObject
{
	GENERATED_BODY()

public:
	// getter
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE FString GetDisplayName() const { return DisplayName; };
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE FString GetDescription() const { return Description; };
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE int32 GetItemTier() const { return ItemTier; };

	FORCEINLINE TObjectPtr<UStaticMesh> GetMeshToSpawn() { return MeshToSpawn; };
	FORCEINLINE TObjectPtr<USoundBase> GetPickUpSound() { return PickUpSound; };

protected:
	// 화면에 보여질 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (DisplayName = "이름"))
	FString DisplayName;
	// 화면에 보여질 아이템 설명
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (DisplayName = "설명", MultiLine = "true"))
	FString Description;
	// 아이템 레어도(0 ~ 4티어)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info",
		meta = (DisplayName = "레어도", ClampMin = "0", ClampMax = "4"))
	int32 ItemTier;
	// 스폰할 때 보여질 메시
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (DisplayName = "스폰 메시"))
	TObjectPtr<UStaticMesh> MeshToSpawn;
	// 아이템 획득 사운드
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (DisplayName = "아이템 획득 사운드"))
	TObjectPtr<USoundBase> PickUpSound;
	// 화면에 보여질 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (DisplayName = "아이템 아이콘"))
	TObjectPtr<UTexture2D> IconTexture;
};
