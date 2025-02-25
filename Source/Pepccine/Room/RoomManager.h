// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RoomManager.generated.h"

class UFloorRoomData;

UENUM()
enum class ERoomType : uint8
{
	ENone,
	EDefault,
	EStart,
	EBoss,
	EItem,
	EShop,
};

USTRUCT()
struct FRoomData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TSoftObjectPtr<UWorld> RoomLevel = nullptr;
	UPROPERTY()
	bool bIsCleared = false;
	UPROPERTY()
	ERoomType RoomType = ERoomType::ENone;
};

UCLASS(DefaultToInstanced, EditInlineNew)
class PEPCCINE_API URoomManager : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Room")
	TArray<UFloorRoomData*> FloorRoomData;
	
	TArray<TArray<FRoomData>> Map;
	FIntPoint CurrentRoom;
	int CurrentFloorIndex;

public:
	void GenerateMap(const TArray<TArray<int>>& Grid);
	void StartFloor();
	
private:
	FRoomData NewRoom(const ERoomType RoomType);

public:
	FORCEINLINE TArray<TArray<FRoomData>>& GetMap(){return Map;}
	FORCEINLINE void SetCurrentRoom(const FIntPoint& InputPoint){CurrentRoom = InputPoint;}
	FORCEINLINE FIntPoint& GetCurrentRoomPoint(){return CurrentRoom;}
	FORCEINLINE TSoftObjectPtr<UWorld>& GetCurrentRoomLevel(){return Map[CurrentRoom.Y][CurrentRoom.X].RoomLevel;}
	FORCEINLINE bool GetCurrentRoomIsClear(){return Map[CurrentRoom.Y][CurrentRoom.X].bIsCleared;}
	FORCEINLINE void SetCurrentRoomIsClear(const bool bInputIsClear){Map[CurrentRoom.Y][CurrentRoom.X].bIsCleared = bInputIsClear;}
	FORCEINLINE int GetCurrentFloor() const {return CurrentFloorIndex + 1;}
};
