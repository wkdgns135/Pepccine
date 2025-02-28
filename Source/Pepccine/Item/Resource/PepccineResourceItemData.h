#pragma once

#include "CoreMinimal.h"
#include "Item/PepccineItemDataBase.h"
#include "PepccineResourceItemData.generated.h"

UENUM(BlueprintType)
enum class EPepccineResourceItemType : uint8
{
	EPRIT_AmmoBox UMETA(DisplayName = "탄약통"),
	EPRIT_Coin UMETA(DisplayName = "코인")
};

UCLASS()
class PEPCCINE_API UPepccineResourceItemData : public UPepccineItemDataBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Item|Resource")
	FORCEINLINE EPepccineResourceItemType GetResourceItemType() const { return ResourceItemType; };

	UFUNCTION(BlueprintPure, Category = "Item|Resource")
	FORCEINLINE int32 GetResourceCount() const { return ResourceCount; };

	FORCEINLINE void SetResourceCount(const int32 InResourceCount) { ResourceCount = InResourceCount; };

protected:
	// 자원 타입
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Resource", meta = (DisplayName = "자원 타입"))
	EPepccineResourceItemType ResourceItemType;

	// 자원 개수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info|Resource", meta = (DisplayName = "자원 개수"))
	int32 ResourceCount = 1;
};
