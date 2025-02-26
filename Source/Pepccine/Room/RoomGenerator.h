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
	bool GenerateEndPoints();
	void InitializeGrid();
	void GenerateRooms();
	void MarkEndRooms();
	void AddAdditionalEndRooms();
	void PrintGrid();
	void AssignEndRooms();
	void StartNextFloor() const;
	TArray<FIntPoint> FindShortestPath(const FIntPoint Start, const FIntPoint End) const;
	
	static void ShuffleArray(TArray<FIntPoint>& Array);
	static int GetLengthBetweenPoint(const FIntPoint& A, const FIntPoint& B);

	FORCEINLINE bool IsPointValid(const FIntPoint& Point) const { return Point.X >= 0 && Point.X < MapSize && Point.Y >= 0 && Point.Y < MapSize; };
};
