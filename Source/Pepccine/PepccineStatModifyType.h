#pragma once

#include "CoreMinimal.h"

#include "PepccineStatModifyType.generated.h"

UENUM(BlueprintType)
enum class EPepccineStatModifyType : uint8
{
	EPSMT_Add UMETA(DisplayName = "합연산"),
	EPSMT_Multiply UMETA(DisplayName = "곱연산")
};

