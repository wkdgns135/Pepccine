#pragma once

#include "CoreMinimal.h"
#include "PlayerStats.h"
#include "Engine/DataAsset.h"
#include "PlayerStatDataAsset.generated.h"

UCLASS(BlueprintType)
class PEPCCINE_API UPlayerStatDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FPlayerStats DefaultStats;
};
