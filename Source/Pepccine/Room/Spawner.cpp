// Fill out your copyright notice in the Description page of Project Settings.


#include "Room/Spawner.h"
#include "PepccineGameState.h"
#include "Room/BaseRoom.h"
#include "BaseMonster.h"
#include "Kismet/GameplayStatics.h"

ASpawner::ASpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	GameState = Cast<APepccineGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState)
	{
		GameState->GetCurrentRoom()->OnRoomStarted.AddUObject(this, &ASpawner::SpawnMonster);
	}
}

void ASpawner::SpawnMonster()
{
	if (SpawnMonsterClass)
	{
		GetWorld()->SpawnActor<AActor>(SpawnMonsterClass, GetActorTransform());
	}
}


