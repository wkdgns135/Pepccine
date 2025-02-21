#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PepccineWeaponItemType.generated.h"

UENUM(BlueprintType)
enum class EPepccineWeaponItemType : uint8
{
	EPWIT_Main UMETA(DisplayName = "주 무기"),
	EPWIT_Sub UMETA(DisplayName = "보조 무기")
};
