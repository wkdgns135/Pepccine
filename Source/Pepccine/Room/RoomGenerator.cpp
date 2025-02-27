#include "Room/RoomGenerator.h"

#include "FloorRoomData.h"
#include "PepccineGameInstance.h"
#include "RoomManager.h"
#include "Kismet/GameplayStatics.h"

class FUnionFind
{
public:
	TArray<int> Parent, Rank;

	explicit FUnionFind(const int InSize)
	{
		Parent.SetNum(InSize);
		Rank.Init(1, InSize);
		for (int i = 0; i < InSize; i++)
		{
			Parent[i] = i;
		}
	}

	int Find(const int X)
	{
		if (Parent[X] == X)
		{
			return X;
		}
		return Parent[X] = Find(Parent[X]);
	}

	void Unite(const int X, const int Y)
	{
		const int RootX = Find(X);
		const int RootY = Find(Y);
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
	EndPointCount = 4;
	LoopThreshold = 1000;
}

void ARoomGenerator::BeginPlay()
{
	Super::BeginPlay();
	GetParameters();
	if (!GenerateEndPoints())
	{
		return;
	}

	GenerateRooms();
	InitializeGrid();
	PrintGrid();
	MarkEndRooms();
	PrintGrid();
	AddAdditionalEndRooms();
	PrintGrid();
	AssignEndRooms();
	PrintGrid();
	StartNextFloor();
}

void ARoomGenerator::GetParameters()
{
	if (const URoomManager *RoomManager = Cast<UPepccineGameInstance>(GetGameInstance())->GetRoomManager())
	{
		if (const UFloorRoomData *FloorRoomData = RoomManager->GetCurrentFloorRoomData())
		{
			EndPointCount = FloorRoomData->EndPointCount;
			MapSize = FloorRoomData->MapSize;
		}
	}
}


bool ARoomGenerator::GenerateEndPoints()
{
	if (MapSize < 3 || EndPointCount <= 0) return false;

	const int DivisionSize = FMath::Max(3, MapSize / FMath::CeilToInt(FMath::Sqrt(static_cast<float>(EndPointCount))));
	TArray<FIntPoint> Divisions;

	for (int X = 0; X < MapSize; X += DivisionSize)
	{
		for (int Y = 0; Y < MapSize; Y += DivisionSize)
		{
			Divisions.Add({ X, Y });
		}
	}

	if (Divisions.Num() < EndPointCount) return false;

	TArray<FIntPoint> SelectedDivisions = Divisions;
	SelectedDivisions.SetNum(EndPointCount);

	for (const FIntPoint& Division : SelectedDivisions)
	{
		int X = FMath::RandRange(Division.X, FMath::Min(Division.X + DivisionSize - 1, MapSize - 1));
		int Y = FMath::RandRange(Division.Y, FMath::Min(Division.Y + DivisionSize - 1, MapSize - 1));

		EndPoints.Push({ X, Y });
	}

	return true;
}

void ARoomGenerator::GenerateRooms()
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

	FUnionFind UnionFind(EndPoints.Num());
	for (const TTuple<int, int, int>& Edge : Edges)
	{
		const int U = Edge.Get<0>();
		const int V = Edge.Get<1>();
		if (UnionFind.Find(U) != UnionFind.Find(V))
		{
			UnionFind.Unite(U, V);
			TArray<FIntPoint> Path = FindShortestPath(EndPoints[U], EndPoints[V]);
			Rooms.Append(Path);
		}
	}
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
	for (const FIntPoint& Room : Rooms)
	{
		Grid[Room.Y][Room.X] = 1;
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

TArray<FIntPoint> ARoomGenerator::FindShortestPath(const FIntPoint Start, const FIntPoint End) const
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

		// 이미 방에 추가된 경로부터 우선적으로 탐색
		for (const FIntPoint& Offset : Offsets)
		{
			FIntPoint Next = Current + Offset;
			if (!CameFrom.Contains(Next) && Rooms.Contains(Next))
			{
				Frontier.Enqueue(Next);
				CameFrom.Add(Next, Current);
			}
		}
		for (const FIntPoint& Offset : Offsets)
		{
			FIntPoint Next = Current + Offset;
			if (!CameFrom.Contains(Next) && !Rooms.Contains(Next))
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

int ARoomGenerator::GetLengthBetweenPoint(const FIntPoint& A, const FIntPoint& B)
{
	return FMath::Abs(A.X - B.X) + FMath::Abs(A.Y - B.Y);
}

void ARoomGenerator::ShuffleArray(TArray<FIntPoint>& Array)
{
	Array.Sort([](const FIntPoint& A, const FIntPoint& B) { return FMath::RandRange(0, 1) == 0; });
}


void ARoomGenerator::AssignEndRooms()
{
	InitializeGrid();
	
	int MaxDistance = 0;
	FIntPoint StartRoom, BossRoom;
	for (int i = 0; i < EndRooms.Num(); i++)
	{
		for (int j = i + 1; j < EndRooms.Num(); j++)
		{
			const int Distance = FindShortestPath(EndRooms[i], EndRooms[j]).Num();
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
	
	// 방 할당 0: 벽, 1: 기본 방, 2: 시작지점, 3: 보스방, 4: 아이템 방, 5: 상점 방
	// 시작 방
	Grid[StartRoom.Y][StartRoom.X] = 2;
	// 보스 방
	Grid[BossRoom.Y][BossRoom.X] = 3;
	// 아이템 방
	Grid[EndRooms[0].Y][EndRooms[0].X] = 4;
	// 상점 방
	Grid[EndRooms[1].Y][EndRooms[1].X] = 5;
}

void ARoomGenerator::StartNextFloor() const
{
	if (UPepccineGameInstance *PepccineGameInstance = Cast<UPepccineGameInstance>(GetGameInstance()))
	{
		PepccineGameInstance->GetRoomManager()->GenerateMap(Grid);
		PepccineGameInstance->GetRoomManager()->StartFloor();
	}
}
