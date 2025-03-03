#include "PepccinePoolSubSystem.h"
#include "PepccinePoolable.h"

void UPepccinePoolSubSystem::SpawnFromPool(const TSubclassOf<APepccinePoolable> PoolClass, const FVector Location,
                                           const FRotator Rotation,
                                           APepccinePoolable*& SpawnedActor)
{
	SpawnedActor = GetActorFromPool(PoolClass, Location, Rotation);
}

void UPepccinePoolSubSystem::ReturnToPool(APepccinePoolable* Poolable)
{
	if (!Poolable) return;

	UClass* ActorClass = Poolable->GetClass();

	Poolable->OnReturnToPool();
	FPoolArray& ObjectPool = ObjectPools.FindOrAdd(ActorClass);
	ObjectPool.Add(Poolable);
}

void UPepccinePoolSubSystem::InitializePool(const TSubclassOf<APepccinePoolable>& PoolClass, const int32 MaxSize,
                                            const FActorSpawnParameters& SpawnParams)
{
	FPoolArray& ObjectPool = ObjectPools.FindOrAdd(PoolClass);
	for (int32 i = 0; i < MaxSize; ++i)
	{
		if (APepccinePoolable* NewActor = GetWorld()->SpawnActor<APepccinePoolable>(
			PoolClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams))
		{
			NewActor->OnReturnToPool();
			ObjectPool.Add(NewActor);
		}
	}
}

APepccinePoolable* UPepccinePoolSubSystem::GetActorFromPool(const TSubclassOf<APepccinePoolable>& PoolClass,
                                                            const FVector& Location,
                                                            const FRotator& Rotation)
{
	FPoolArray& ObjectPool = ObjectPools.FindOrAdd(PoolClass);
	if (!ObjectPool.IsEmpty())
	{
		if (APepccinePoolable* Actor = ObjectPool.Pop())
		{
			Actor->SetActorLocationAndRotation(Location, Rotation);
			Actor->OnSpawnFromPool();
			return Actor;
		}
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (APepccinePoolable* NewActor = GetWorld()->SpawnActor<APepccinePoolable>(
		PoolClass, Location, Rotation, SpawnParams))
	{
		NewActor->OnSpawnFromPool();
		return NewActor;
	}

	return nullptr;
}
