#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PepccineItemSubManager.generated.h"

class UPepccineItemManagerComponent;

UCLASS()
class PEPCCINE_API UPepccineItemSubManager : public UObject
{
	GENERATED_BODY()

public:
	// 매니저 초기화
	FORCEINLINE void InitializeManager(UPepccineItemManagerComponent* InItemManager) { ItemManager = InItemManager; }

protected:
	UPROPERTY()
	UPepccineItemManagerComponent* ItemManager;
};
