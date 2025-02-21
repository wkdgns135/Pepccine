// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomGenerator.generated.h"

UCLASS()
class PEPCCINE_API ARoomGenerator : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	int MapSize; // MapSize > 6
	UPROPERTY(EditAnywhere)
	int EndPointCount; // EndPointCount <= (MapSize + MapSize) / 3
	UPROPERTY(EditAnywhere)
	int LoopThreshold;

	TArray<FIntPoint> EndPoints; // {{X, Y}}
	TArray<TArray<int>> Grid;
	TSet<FIntPoint> Rooms;
	TArray<FIntPoint> EndRooms;

public:	
	ARoomGenerator();

protected:
	virtual void BeginPlay() override;

private:
	void GetParameters();
	bool GetFindNeighbor(const FIntPoint& Point);
	bool GenerateEndPoints();
	void InitializeGrid();
	void GenerateMST();
	void MarkEndRooms();
	void AddAdditionalEndRooms();
	void PrintGrid();
	TArray<FIntPoint> FindShortestPath(FIntPoint Start, FIntPoint End);
	void AssignEndRoom();

	FORCEINLINE bool IsPointValid(const FIntPoint& Point) { return Point.X >= 0 && Point.X < MapSize && Point.Y >= 0 && Point.Y < MapSize; };
	FORCEINLINE void ShuffleArray(TArray<FIntPoint>& Array) { Array.Sort([](const FIntPoint& A, const FIntPoint& B) { return FMath::RandRange(0, 1) == 0; }); };
	FORCEINLINE int GetLengthBetweenPoint(const FIntPoint& A, const FIntPoint& B) { return FMath::Abs(A.X - B.X) + FMath::Abs(A.Y - B.Y); };
	
};
