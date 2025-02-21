#include "Room/RoomGenerator.h"

class FUnionFind
{
public:
	TArray<int> Parent, Rank;

	FUnionFind(int InSize)
	{
		Parent.SetNum(InSize);
		Rank.SetNum(InSize, 1);
		for (int i = 0; i < InSize; i++)
		{
			Parent[i] = i;
		}
	}

	int Find(int X)
	{
		if (Parent[X] == X)
		{
			return X;
		}
		return Parent[X] = Find(Parent[X]);
	}

	void Unite(int X, int Y)
	{
		int RootX = Find(X);
		int RootY = Find(Y);
		if (RootX != RootY)
		{
			if (Rank[RootX] > Rank[RootY])
			{
				Parent[RootY] = RootX;
			}
			else if (Rank[RootX] < Rank[RootY])
			{
				Parent[RootX] = RootY;
			}
			else
			{
				Parent[RootY] = RootX;
				Rank[RootX]++;
			}
		}
	}
};

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
	if (!GenerateEndPoints())
	{
		return;
	}

	GenerateMST();

	if (!Rooms.IsEmpty())
	{
		UE_LOG(LogTemp, Display, TEXT("Generate finish"));
		PrintGrid();
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Failed to generate"));
	}
}

void ARoomGenerator::GetParameters()
{
	// Implementation here
}

bool ARoomGenerator::GetFindNeighbor(const FIntPoint& Point)
{
	TArray<FIntPoint> Offsets = { {0,0},{0,1},{0,-1},{1,0},{-1,0},{1,1},{1,-1},{-1,1},{-1,-1} };
	for (const FIntPoint& Offset : Offsets)
	{
		FIntPoint Next = Point + Offset;
		if (EndPoints.Contains(Next))
		{
			return true;
		}
	}
	return false;
}

bool ARoomGenerator::GenerateEndPoints()
{
	if (MapSize < 3 || EndPointCount <= 0) return false;

	int32 DivisionSize = FMath::Max(3, MapSize / FMath::CeilToInt(FMath::Sqrt((float)EndPointCount)));
	TArray<FIntPoint> Divisions;

	for (int32 X = 0; X < MapSize; X += DivisionSize)
	{
		for (int32 Y = 0; Y < MapSize; Y += DivisionSize)
		{
			Divisions.Add({ X, Y });
		}
	}

	if (Divisions.Num() < EndPointCount) return false;

	TArray<FIntPoint> SelectedDivisions = Divisions;
	SelectedDivisions.SetNum(EndPointCount);

	for (const FIntPoint& Division : SelectedDivisions)
	{
		int32 X = FMath::RandRange(Division.X, FMath::Min(Division.X + DivisionSize - 1, MapSize - 1));
		int32 Y = FMath::RandRange(Division.Y, FMath::Min(Division.Y + DivisionSize - 1, MapSize - 1));

		EndPoints.Push({ X, Y });
	}

	return true;
}

void ARoomGenerator::GenerateMST()
{
	TArray<TTuple<int, int, int>> Edges;
	for (int i = 0; i < EndPoints.Num(); i++)
	{
		for (int j = i + 1; j < EndPoints.Num(); j++)
		{
			Edges.Add({ i, j, GetLengthBetweenPoint(EndPoints[i], EndPoints[j])});
		}
	}

	Edges.Sort([](const TTuple<int, int, int>& A, const TTuple<int, int, int>& B) { return A.Get<2>() < B.Get<2>(); });

	FUnionFind UF(EndPoints.Num());
	for (const TTuple<int, int, int>& Edge : Edges)
	{
		int U = Edge.Get<0>();
		int V = Edge.Get<1>();
		if (UF.Find(U) != UF.Find(V))
		{
			UF.Unite(U, V);
			TArray<FIntPoint> Path = FindShortestPath(EndPoints[U], EndPoints[V]);
			Rooms.Append(Path);
		}
	}

	InitializeGrid();

	for (const FIntPoint& Point : Rooms)
	{
		Grid[Point.Y][Point.X] = 1;
	}

	PrintGrid();

	MarkEndRooms();

	PrintGrid();

	AddAdditionalEndRooms();
}

void ARoomGenerator::InitializeGrid()
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
}

void ARoomGenerator::MarkEndRooms()
{
	TArray<FIntPoint> Offsets = { {0,1},{0,-1},{1,0},{-1,0} };
	for (const FIntPoint& Point : Rooms)
	{
		int NeighborCount = 0;
		for (const FIntPoint& Offset : Offsets)
		{
			FIntPoint Next = Point + Offset;
			if (IsPointValid(Next) && Grid[Next.Y][Next.X] >= 1)
			{
				NeighborCount++;
			}
		}
		if (NeighborCount == 1)
		{
			Grid[Point.Y][Point.X] = 2;
			EndRooms.Add(Point);
		}
	}
}

void ARoomGenerator::AddAdditionalEndRooms()
{
	if (EndRooms.Num() < EndPointCount)
	{
		TArray<FIntPoint> RemainingPositions = Rooms.Array();
		RemainingPositions.RemoveAll([&](const FIntPoint& Point) { return Grid[Point.Y][Point.X] == 2; });
		
		ShuffleArray(RemainingPositions);
		TArray<FIntPoint> Offsets = { {0,1},{0,-1},{1,0},{-1,0} };
		for (const FIntPoint& Point : RemainingPositions)
		{
			if (EndRooms.Num() == EndPointCount)
			{
				break;
			}
			ShuffleArray(Offsets);
			for (const FIntPoint& Offset : Offsets)
			{
				FIntPoint Next = Point + Offset;
				if (IsPointValid(Next) && Grid[Next.Y][Next.X] == 0)
				{
					int NeighborCount = 0;
					for (const FIntPoint& Offset2 : Offsets)
					{
						FIntPoint NextNext = Next + Offset2;
						if (IsPointValid(NextNext) && Grid[NextNext.Y][NextNext.X] >= 1)
						{
							NeighborCount++;
						}
					}
					if (NeighborCount == 1)
					{
						Grid[Next.Y][Next.X] = 2;
						Rooms.Add(Next);
						EndRooms.Add(Next);
						break;
					}
				}
			}
		}
	}
}

void ARoomGenerator::PrintGrid()
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

TArray<FIntPoint> ARoomGenerator::FindShortestPath(FIntPoint Start, FIntPoint End)
{
	TArray<FIntPoint> Path;
	TMap<FIntPoint, FIntPoint> CameFrom;
	TQueue<FIntPoint> Frontier;
	Frontier.Enqueue(Start);
	CameFrom.Add(Start, Start);

	TArray<FIntPoint> Offsets = { {0,1},{0,-1},{1,0},{-1,0} };

	while (!Frontier.IsEmpty())
	{
		FIntPoint Current;
		Frontier.Dequeue(Current);

		if (Current == End)
		{
			break;
		}

		for (const FIntPoint& Offset : Offsets)
		{
			FIntPoint Next = Current + Offset;
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

void ARoomGenerator::AssignEndRooms()
{
	// HERE [장훈]: 특수방 생성 로직 
	ShuffleArray(EndRooms);

	int MaxDistance = 0;
	FIntPoint StartRoom, BossRoom;

	for (int i = 0; i < EndRooms.Num(); i++)
	{
		for (int j = i + 1; j < EndRooms.Num(); j++)
		{
			int Distance = GetLengthBetweenPoint(EndRooms[i], EndRooms[j]);
			if (Distance > MaxDistance)
			{
				MaxDistance = Distance;
				StartRoom = EndRooms[i];
				BossRoom = EndRooms[j];
			}
		}
	}

	EndRooms.Remove(StartRoom);
	EndRooms.Remove(BossRoom);
}
