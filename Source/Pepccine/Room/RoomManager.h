// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RoomManager.generated.h"

class UPepccineItemDataBase;
class ULevelStreamingDynamic;
enum class EDoorDirection : uint8;
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
	bool bIsLocked = false;
	UPROPERTY()
	ERoomType RoomType = ERoomType::ENone;
	UPROPERTY()
	FIntPoint RoomPoint;
	UPROPERTY()
	ULevelStreamingDynamic *StreamingLevel = nullptr;
	UPROPERTY()
	TMap<UPepccineItemDataBase*, FVector> ItemData;
};

UCLASS(DefaultToInstanced, EditInlineNew)
class PEPCCINE_API URoomManager : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Room")
	TArray<UFloorRoomData*> FloorRoomData;
	
	TArray<TArray<FRoomData*>> Map;
	FRoomData *CurrentRoom;
	FRoomData *PreviousRoom;
	int CurrentFloorIndex;

public:
	void GenerateMap(const TArray<TArray<int>>& Grid);
	void StartFloor();
	void ChangeRoom(FRoomData* RoomData);
	bool GetCheckRoom(const FIntPoint Point) const;
	void NextFloor();
	
private:
	FRoomData* NewRoom(const ERoomType RoomType);
	void PrintFloor() const;
	
public:
	FORCEINLINE TArray<TArray<FRoomData*>>& GetMap(){return Map;}
	FORCEINLINE FIntPoint GetCurrentRoomPoint() const {return CurrentRoom ? CurrentRoom->RoomPoint : FIntPoint();}
	FORCEINLINE int GetCurrentFloorIndex() const {return CurrentFloorIndex + 1;}
	FORCEINLINE FRoomData* GetCurrentRoomData() const {return CurrentRoom;}
	FORCEINLINE FRoomData* GetPointRoomData(const FIntPoint& Point){return GetCheckRoom(Point) ? Map[Point.Y][Point.X] : NewRoom(ERoomType::ENone);}
	FORCEINLINE FRoomData* GetPreviousRoomData() const {return PreviousRoom;}
	FORCEINLINE UFloorRoomData* GetCurrentFloorRoomData() const {return FloorRoomData[CurrentFloorIndex];}
};
