// Fill out your copyright notice in the Description page of Project Settings.


#include "PepccineGameInstance.h"
#include "Room/RoomManager.h"

void UPepccineGameInstance::Init()
{
	Super::Init();
	RoomManager = NewObject<URoomManager>(this);
}
