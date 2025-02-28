// Fill out your copyright notice in the Description page of Project Settings.


#include "Room/Spawner.h"

#include "PepccineGameInstance.h"
#include "PepccineGameState.h"
#include "RoomManager.h"
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
			RoomController->OnRoomStarted.AddUObject(this, &ASpawner::SpawnMonster);
		}
	}
}

void ASpawner::SpawnMonster()
{
	if (SpawnMonsterClass)
	{
		GetWorld()->SpawnActor<AActor>(SpawnMonsterClass, GetActorTransform());
		if (APepccineGameState* PepccineGameState = Cast<APepccineGameState>(UGameplayStatics::GetGameState(GetWorld())))
		{
			if (ABaseRoomController *RoomController = PepccineGameState->GetRoomController())
			{
				RoomController->IncreaseMonsterCount();
			}
		}
	}
	
}


