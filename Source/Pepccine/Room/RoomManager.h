// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RoomManager.generated.h"

/**
 * 
 */
UCLASS()
class PEPCCINE_API URoomManager : public UObject
{
	GENERATED_BODY()

private:
	TArray<TArray<TPair<int, bool>>> Map; // {{Type, bIsClear}}
	FIntPoint CurrentRoom;
	int CurrentFloor;
	
public:
	FORCEINLINE void SetMap(const TArray<TArray<TPair<int, bool>>>& InputMap){Map = InputMap;}
	FORCEINLINE TArray<TArray<TPair<int, bool>>>& GetMap(){return Map;}
	FORCEINLINE void SetCurrentRoom(const FIntPoint& InputPoint){CurrentRoom = InputPoint;}
	FORCEINLINE FIntPoint& GetCurrentRoomPoint(){return CurrentRoom;}
	FORCEINLINE bool GetCurrentRoomIsClear(){return Map[CurrentRoom.Y][CurrentRoom.X].Value;}
	FORCEINLINE void SetCurrentRoomIsClear(const bool bInputIsClear){Map[CurrentRoom.Y][CurrentRoom.X].Value = bInputIsClear;}
	FORCEINLINE void SetCurrentFloor(const int InputFloor){CurrentFloor = InputFloor;}
	FORCEINLINE int GetCurrentFloor() const {return CurrentFloor;}
};
