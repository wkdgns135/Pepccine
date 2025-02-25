#pragma once

#include "CoreMinimal.h"

#include "PepccineCharacterFeature.generated.h"

UENUM(BlueprintType)
enum class EPepccineCharacterFeatureName : uint8
{
	EPCFN_Roll UMETA(DisplayName = "구르기"),
	EPCFN_Sprint UMETA(DisplayName = "달리기")
};

USTRUCT(BlueprintType)
struct FPepccineCharacterFeature
{
	GENERATED_BODY()

	// 캐릭터 기능 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modifier",
		meta = (DisplayName = "캐릭터 추가 기능 이름"))
	EPepccineCharacterFeatureName CharacterFeatureName;

	FPepccineCharacterFeature(): CharacterFeatureName(EPepccineCharacterFeatureName::EPCFN_Roll)
	{
	}
};