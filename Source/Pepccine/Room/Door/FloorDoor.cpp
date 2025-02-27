// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorDoor.h"

#include "PepccineGameInstance.h"
#include "Room/RoomManager.h"


AFloorDoor::AFloorDoor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFloorDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFloorDoor::EnterDoor()
{
	// TODO [장훈]: 다음층 진입 로직 추가
	URoomManager* RoomManager = Cast<UPepccineGameInstance>(GetGameInstance())->GetRoomManager();
	if (RoomManager)
	{
		RoomManager->NextFloor();
	}
}

void AFloorDoor::OnStarted()
{
	
}


