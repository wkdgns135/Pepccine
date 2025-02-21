// Fill out your copyright notice in the Description page of Project Settings.


#include "Room/RoomGenerator.h"

ARoomGenerator::ARoomGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	MapSize = 6;
	EndPointCount = 3;
	LoopThreshold = 1000;
}

void ARoomGenerator::BeginPlay()
{
	Super::BeginPlay();

	GenerateEndPoints();
	BackTracking(0);
}

void ARoomGenerator::GetParameters()
{
}

bool ARoomGenerator::GetFindNeighbor(int X, int Y)
{
	int Offset[9][2] = { {0,0},{0,1},{0,-1},{1,0},{-1,0},{1,1},{1,-1},{-1,1},{-1,-1} };
	for (int i = 0; i < 9; i++)
	{
		int XX = X + Offset[i][0];
		int YY = Y + Offset[i][1];
		for (const TTuple<int, int, int>& EndPoint : EndPoints)
		{
			if (XX == EndPoint.Get<0>() && YY == EndPoint.Get<1>())
			{
				return true;
			}
		}
	}
	return false;
}

bool ARoomGenerator::GenerateEndPoints()
{
	for (int i = 0; i < EndPointCount; i++)
	{
		int Count = 0;
		while (Count < LoopThreshold)
		{
			Count++;
			int X = FMath::RandRange(1, MapSize - 2);
			int Y = FMath::RandRange(1, MapSize - 2);
			int Direction = FMath::RandRange(0, 3);

			if (!GetFindNeighbor(X, Y))
			{
				EndPoints.Push({ X, Y , -1});
				break;
			}
		}
		if (Count >= LoopThreshold)
		{
			return false;
		}
		
	}
	return true;
}

void ARoomGenerator::BackTracking(int Depth)
{
	if (!RoomPositions.IsEmpty())return;
	if (Depth == EndPointCount)
	{
		GenerateGrid();
		PrintGrid();
		if (!CheckEndPointsConnect())return;
		RoomPositions = FindAllEndPointsShortestPath();
		UE_LOG(LogTemp, Display, TEXT("Generate finish"));
		PrintRoom();
	}
	for (int i = 0; i < EndPointCount; i++)
	{
		TTuple<int, int, int> &Node = EndPoints[i];
		if (Node.Get<2>() != -1)continue;
		for (int j = 0; j < 4; j++)
		{
			Node.Get<2>() = j;
			BackTracking(Depth + 1);
			Node.Get<2>() = -1;
		}
	}
}

void ARoomGenerator::PrintGrid() const
{
	for (int Y = 0; Y < MapSize; Y++)
	{
		FString Row;
		for (int X = 0; X < MapSize; X++)
		{
			Row += FString::Printf(TEXT("%2d "), Grid[Y][X]);
		}
		UE_LOG(LogTemp, Display, TEXT("%s"), *Row);
	}
	UE_LOG(LogTemp, Display, TEXT(" "));
}

void ARoomGenerator::PrintRoom()
{
	Grid.SetNum(MapSize);
	for (int i = 0; i < MapSize; i++)
	{
		Grid[i].SetNum(MapSize);
		for (int j = 0; j < MapSize; j++)
		{
			Grid[i][j] = 0;
		}
	}
	for (const FIntPoint& Point : RoomPositions)
	{
		Grid[Point.Y][Point.X] = 1;
	}
	PrintGrid();
}

void ARoomGenerator::GenerateGrid()
{
	Grid.SetNum(MapSize);
	for (int i = 0; i < MapSize; i++)
	{
		Grid[i].SetNum(MapSize);
		for (int j = 0; j < MapSize; j++)
		{
			Grid[i][j] = 0;
		}
	}

	int Offset[4][2] = { {0,1},{0,-1},{1,0},{-1,0} };
	for (const TTuple<int, int, int> EndPoint : EndPoints)
	{
		int EndPointX = EndPoint.Get<0>();
		int EndPointY = EndPoint.Get<1>();
		int EndPointDirection = EndPoint.Get<2>();
		Grid[EndPointY][EndPointX] = 1;

		for (int i = 0; i < 4; i++)
		{
			if (EndPointDirection == i)continue;
			int X = EndPointX + Offset[i][0];
			int Y = EndPointY + Offset[i][1];
			Grid[Y][X] = -1;
		}
	}
}

bool ARoomGenerator::CheckEndPointsConnect()
{
	TArray<FIntPoint> Visited;
	TQueue<FIntPoint> Queue;

	if (EndPoints.Num() == 0)
	{
		return true;
	}

	FIntPoint Start(EndPoints[0].Get<0>(), EndPoints[0].Get<1>());
	Queue.Enqueue(Start);
	Visited.Add(Start);

	int Offset[4][2] = { {0,1},{0,-1},{1,0},{-1,0} };

	while (!Queue.IsEmpty())
	{
		FIntPoint Current;
		Queue.Dequeue(Current);

		for (int i = 0; i < 4; i++)
		{
			FIntPoint Next(Current.X + Offset[i][0], Current.Y + Offset[i][1]);

			if (Next.X < 0 || Next.X >= MapSize || Next.Y < 0 || Next.Y >= MapSize)
			{
				continue;
			}

			if (Grid[Next.Y][Next.X] == -1)
			{
				continue;
			}

			if (!Visited.Contains(Next))
			{
				Queue.Enqueue(Next);
				Visited.Add(Next);
			}
		}
	}

	for (const TTuple<int, int, int>& EndPoint : EndPoints)
	{
		FIntPoint Point(EndPoint.Get<0>(), EndPoint.Get<1>());
		if (!Visited.Contains(Point))
		{
			return false;
		}
	}

	return true;
}

TArray<FIntPoint> ARoomGenerator::FindShortestPath(FIntPoint Start, FIntPoint End)
{
	TArray<FIntPoint> Path;
	TMap<FIntPoint, FIntPoint> CameFrom;
	TQueue<FIntPoint> Frontier;
	Frontier.Enqueue(Start);
	CameFrom.Add(Start, Start);

	int Offset[4][2] = { {0,1},{0,-1},{1,0},{-1,0} };

	while (!Frontier.IsEmpty())
	{
		FIntPoint Current;
		Frontier.Dequeue(Current);

		if (Current == End)
		{
			break;
		}

		for (int i = 0; i < 4; i++)
		{
			FIntPoint Next(Current.X + Offset[i][0], Current.Y + Offset[i][1]);

			if (Next.X < 0 || Next.X >= MapSize || Next.Y < 0 || Next.Y >= MapSize)
			{
				continue;
			}

			if (Grid[Next.Y][Next.X] == -1)
			{
				continue;
			}

			if (!CameFrom.Contains(Next))
			{
				Frontier.Enqueue(Next);
				CameFrom.Add(Next, Current);
			}
		}
	}

	FIntPoint Current = End;
	while (Current != Start)
	{
		Path.Add(Current);
		Current = CameFrom[Current];
	}
	Path.Add(Start);
	Algo::Reverse(Path);

	return Path;
}

TArray<FIntPoint> ARoomGenerator::FindAllEndPointsShortestPath()
{
	TArray<FIntPoint> AllPaths;
	TArray<FIntPoint> EndPointPositions;

	for (const TTuple<int, int, int>& EndPoint : EndPoints)
	{
		EndPointPositions.Add(FIntPoint(EndPoint.Get<0>(), EndPoint.Get<1>()));
	}

	for (int i = 0; i < EndPointPositions.Num() - 1; i++)
	{
		TArray<FIntPoint> Path = FindShortestPath(EndPointPositions[i], EndPointPositions[i + 1]);
		AllPaths.Append(Path);
		for (int j = i + 1; j < EndPointPositions.Num(); j++)
		{
		}
	}

	return AllPaths;
}
