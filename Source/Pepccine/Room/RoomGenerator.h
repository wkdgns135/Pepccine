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
	int MapSize; // MapSize > 5
	UPROPERTY(EditAnywhere)
	int EndPointCount; // EndPointCount <= MapSize / 2
	UPROPERTY(EditAnywhere)
	int LoopThreshold;

	TArray<TTuple<int, int, int>> EndPoints; // {{X, Y}}
	TArray<TArray<int>> Grid;
	TArray<FIntPoint> RoomPositions;

public:	
	ARoomGenerator();

protected:
	virtual void BeginPlay() override;

private:
	void GetParameters();
	bool GetFindNeighbor(int X, int Y);
	void BackTracking(int Depth);
	bool GenerateEndPoints();
	void GenerateGrid();
	bool CheckEndPointsConnect();
	void PrintGrid() const;
	void PrintRoom();
	TArray<FIntPoint> FindShortestPath(FIntPoint Start, FIntPoint End);
	TArray<FIntPoint> FindAllEndPointsShortestPath();
};
