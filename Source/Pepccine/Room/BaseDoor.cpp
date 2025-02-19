// Fill out your copyright notice in the Description page of Project Settings.


#include "Room/BaseDoor.h"
#include "PepccineGameState.h"
#include "Room/BaseRoom.h"
#include "Kismet/GameplayStatics.h"


ABaseDoor::ABaseDoor()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseDoor::BeginPlay()
{
	Super::BeginPlay();
	APepccineGameState* GameState = Cast<APepccineGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState)
	{
		GameState->GetCurrentRoom()->OnRoomStarted.AddUObject(this, &ABaseDoor::LockDoor);
		GameState->GetCurrentRoom()->OnRoomCleared.AddUObject(this, &ABaseDoor::UnlockDoor);
	}
}

void ABaseDoor::LockDoor()
{
}

void ABaseDoor::UnlockDoor()
{
}

