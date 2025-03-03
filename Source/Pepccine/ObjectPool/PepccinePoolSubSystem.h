#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "PepccinePoolSubSystem.generated.h"

class APepccinePoolable;

USTRUCT()
struct FPoolArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<APepccinePoolable*> ObjectPool;

	bool IsEmpty() const
	{
		return ObjectPool.IsEmpty();
	}

	APepccinePoolable* Pop()
	{
		return ObjectPool.Pop();
	}

	void Add(APepccinePoolable* ActorToAdd)
	{
		ObjectPool.Add(ActorToAdd);
	}
};

UCLASS()
class PEPCCINE_API UPepccinePoolSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "ObjectPool")
	void SpawnFromPool(TSubclassOf<APepccinePoolable> PoolClass, FVector Location, FRotator Rotation,
	                   APepccinePoolable*& SpawnedActor);

	UFUNCTION(BlueprintCallable, Category = "ObjectPool")
	void ReturnToPool(APepccinePoolable* Poolable);

	// UFUNCTION(BlueprintCallable, Category = "ObjectPool")
	void InitializePool(const TSubclassOf<APepccinePoolable>& PoolClass, int32 MaxSize, const FActorSpawnParameters& SpawnParams);


	UFUNCTION()
	APepccinePoolable* GetActorFromPool(const TSubclassOf<APepccinePoolable>& PoolClass, const FVector& Location,
	                                    const FRotator& Rotation);

private:
	TMap<UClass*, FPoolArray> ObjectPools;
};
