#pragma once

#include "CoreMinimal.h"

#include "PepccineItemDataBase.generated.h"

UCLASS(DefaultToInstanced, EditInlineNew)
class PEPCCINE_API UPepccineItemDataBase : public UObject
{
	GENERATED_BODY()

public:
	// getter
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE int32 GetItemId() const { return ItemId; }

	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE FString GetDisplayName() const { return DisplayName; };
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE FString GetDescription() const { return Description; };
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE int32 GetItemTier() const { return ItemTier; };

	FORCEINLINE UStaticMesh* GetMeshToSpawn() { return MeshToSpawn; };
	FORCEINLINE FRotator GetMeshRotationToSpawn() const {return MeshRotationToSpawn; };
	FORCEINLINE FVector GetMeshScaleToSpawn() const { return MeshScaleToSpawn; };
	FORCEINLINE USoundBase* GetPickUpSound() { return PickUpSound; };

	// setter
	FORCEINLINE void SetItemId(const int32 Id) { ItemId = Id; };
	FORCEINLINE void SetMeshToSpawn(UStaticMesh* Mesh) { MeshToSpawn = Mesh; };

protected:
	// 아이템 아이디
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Info", meta = (DisplayName = "아이템 아이디"))
	int32 ItemId;

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
	// 스폰 메시 회전
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (DisplayName = "스폰 메시 회전"))
	FRotator MeshRotationToSpawn = FRotator::ZeroRotator;
	// 스폰 메시 스케일
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (DisplayName = "스폰 메시 스케일"))
	FVector MeshScaleToSpawn = FVector::OneVector;
	// 아이템 획득 사운드
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (DisplayName = "아이템 획득 사운드"))
	TObjectPtr<USoundBase> PickUpSound;

public:
	// 화면에 보여질 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (DisplayName = "아이템 아이콘"))
	TObjectPtr<UTexture2D> IconTexture;
};
