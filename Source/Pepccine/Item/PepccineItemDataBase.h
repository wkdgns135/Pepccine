#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "PepccineItemDataBase.generated.h"

UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class PEPCCINE_API UPepccineItemDataBase : public UObject
{
	GENERATED_BODY()
	
public:
	// 화면에 보여질 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (DisplayName = "이름"))
	FString DisplayName;
	// 화면에 보여질 아이템 설명
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (DisplayName = "설명", MultiLine = "true"))
	FString Description;
	// 아이템 레어도(0 ~ 4티어)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (DisplayName = "레어도", ClampMin = "0", ClampMax = "4"))
	int32 ItemTier;
	// 판매할 때 가격
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (DisplayName = "판매 가격"))
	int32 SellingPrice;
	// 스폰할 때 보여질 메시
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (DisplayName = "스폰 메시"))
	TObjectPtr<UStaticMesh> MeshToSpawn;
};
