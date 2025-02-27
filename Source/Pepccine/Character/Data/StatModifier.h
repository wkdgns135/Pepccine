#pragma once

#include "CoreMinimal.h"
#include "Item/PepccineStatName.h"
#include "StatModifier.generated.h"

USTRUCT(BlueprintType)
struct FStatModifier
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPepccineCharacterStatName StatType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AdditiveValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MultiplicativeValue = 1.0f;

	FStatModifier()
			: StatType(EPepccineCharacterStatName::EPCSN_AttackDamage),
				AdditiveValue(0.0f),
				MultiplicativeValue(1.0f)
	{}

	FStatModifier(EPepccineCharacterStatName InStatType, float InAdditive, float InMultiplicative)
			: StatType(InStatType),
				AdditiveValue(InAdditive),
				MultiplicativeValue(InMultiplicative)
	{}

	bool operator==(const FStatModifier& Other) const
	{
		return StatType == Other.StatType &&
					 FMath::IsNearlyEqual(AdditiveValue, Other.AdditiveValue) &&
					 FMath::IsNearlyEqual(MultiplicativeValue, Other.MultiplicativeValue);
	}
};
