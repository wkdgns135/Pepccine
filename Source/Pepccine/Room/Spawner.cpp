// Fill out your copyright notice in the Description page of Project Settings.


#include "Room/Spawner.h"
#include "PepccineGameState.h"
#include "Controller/BaseRoomController.h"
#include "Monster/Class/BaseMonster.h"
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
		ABaseRoomController* RoomController = GameState->GetRoomController();
		if (RoomController)
		{
			RoomController->OnRoomCleared.AddUObject(this, &ASpawner::SpawnMonster);
		}
	}
}

void ASpawner::SpawnMonster()
{
	if (SpawnMonsterClass)
	{
		GetWorld()->SpawnActor<AActor>(SpawnMonsterClass, GetActorTransform());
	}
}


