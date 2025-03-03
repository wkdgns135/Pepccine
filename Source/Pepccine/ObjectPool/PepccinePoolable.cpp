#include "PepccinePoolable.h"

void APepccinePoolable::OnSpawnFromPool()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void APepccinePoolable::OnReturnToPool()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}
