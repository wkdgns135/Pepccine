#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CharacterSaveManager.generated.h"

struct FModifier;

UCLASS()
class PEPCCINE_API UCharacterSaveManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Stats")
	bool LoadPlayerStats(FPlayerStats& OutStats, TArray<FStatModifier>& OutModifiers, FPlayerStats& OutTotalAdd, FPlayerStats& OutTotalMul);
	
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SavePlayerStats(const FPlayerStats& InStats, const TArray<FStatModifier>& InModifiers, FPlayerStats InTotalAdd, FPlayerStats InTotalMul);

	bool IsFirstTimeLoaded() const { return bIsFirstTimeLoaded; }

	FORCEINLINE_DEBUGGABLE bool GetIsFirstTimeLoaded() const { return bIsFirstTimeLoaded; }
	FORCEINLINE_DEBUGGABLE void SetIsFirstTimeLoaded(const bool b) { bIsFirstTimeLoaded = b; }
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	bool bIsFirstTimeLoaded = false;
};
