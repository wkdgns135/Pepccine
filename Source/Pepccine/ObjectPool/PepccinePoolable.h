#pragma once

#include "CoreMinimal.h"

#include "PepccinePoolable.generated.h"

UCLASS(Abstract)
class PEPCCINE_API APepccinePoolable : public AActor
{
	GENERATED_BODY()

public:
	// 풀에서 스폰
	UFUNCTION(BlueprintCallable, Category = "ObjectPool")
	virtual void OnSpawnFromPool();

	// 풀로 돌아가기
	UFUNCTION(BlueprintCallable, Category = "ObjectPool")
	virtual void OnReturnToPool();
};
