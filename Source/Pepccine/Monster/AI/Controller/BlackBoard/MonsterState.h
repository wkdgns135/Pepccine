#pragma once

#include "CoreMinimal.h"
#include "MonsterState.generated.h"

// Enum declaration with UENUM for Blueprint exposure
UENUM(BlueprintType)  // Expose to Blueprints
enum class EMonsterState : uint8  // Use 'E' prefix for enums to follow C++ convention
{
    Idle        UMETA(DisplayName = "Idle"),
    Attacking   UMETA(DisplayName = "Attacking"),
    Frozen      UMETA(DisplayName = "Frozen"),
    Dead        UMETA(DisplayName = "Dead")
};
