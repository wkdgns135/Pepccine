#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerStats.h"
#include "Character/Data/StatModifier.h"
#include "PlayerStatSaveGame.generated.h"

UCLASS()
class PEPCCINE_API UPlayerStatSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category = "Player Stats")
	FPlayerStats SavedStats;
	TArray<FStatModifier> ActiveModifiers;
	FPlayerStats CurrentTotalAdd;
	FPlayerStats CurrentTotalMul;
	
	UPlayerStatSaveGame()
	{
		SavedStats = FPlayerStats();
	}
};
