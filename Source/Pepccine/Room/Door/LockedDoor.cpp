// Fill out your copyright notice in the Description page of Project Settings.


#include "LockedDoor.h"

ALockedDoor::ALockedDoor()
{
	PrimaryActorTick.bCanEverTick = false;
}


void ALockedDoor::EnterDoor()
{
	GetDirectionRoom();
}
