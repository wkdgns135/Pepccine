#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // FTableRowBase 정의가 들어있는 헤더
#include "ItemDataRow.generated.h"

USTRUCT(BlueprintType)
struct FItemDataRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackMultiplier;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RangeMultiplier;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FireRateMultiplier;
};
