// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomManager.h"

#include "FloorRoomData.h"
#include "Kismet/GameplayStatics.h"

void URoomManager::GenerateMap(const TArray<TArray<int>>& Grid)
{
	Map.SetNum(Grid.Num());
	for (int i = 0; i < Grid.Num(); i++)
	{
		Map[i].SetNum(Grid[i].Num());
		for (int j = 0; j < Grid[i].Num(); j++)
		{
			switch (Grid[i][j])
			{
			default: Map[i][j] = NewRoom(ERoomType::ENone); break;
			case 1 : Map[i][j] = NewRoom(ERoomType::EDefault); break;
			case 2 : Map[i][j] = NewRoom(ERoomType::EStart); break;
			case 3 : Map[i][j] = NewRoom(ERoomType::EBoss); break;
			case 4 : Map[i][j] = NewRoom(ERoomType::EItem); break;
			case 5 : Map[i][j] = NewRoom(ERoomType::EShop); break;
			}
			
			Map[i][j]->RoomPoint = FIntPoint(j, i);
			
			if (Map[i][j]->RoomType == ERoomType::EStart)
			{
				CurrentRoom = Map[i][j];
			}
			else if (Map[i][j]->RoomType == ERoomType::EShop)
			{
				Map[i][j]->bIsLocked = true;
			}
			else if (Map[i][j]->RoomType == ERoomType::EItem)
			{
				Map[i][j]->bIsLocked = true;
			}
		}
	}
}

void URoomManager::StartFloor()
{
	ChangeRoom(GetCurrentRoomData());
}

bool URoomManager::GetCheckRoom(const FIntPoint Point) const
{
	if (Map.IsValidIndex(Point.Y) && Map[Point.Y].IsValidIndex(Point.X))
	{
		return Map[Point.Y][Point.X]->RoomType != ERoomType::ENone;
	}
	return false;
}

void URoomManager::NextFloor()
{
	if (CurrentFloorIndex >= FloorRoomData.Num() - 1) return;
	CurrentFloorIndex++;
	UGameplayStatics::OpenLevel(GetWorld(), "GenerateRoomLevel");
}

void URoomManager::ChangeRoom(FRoomData* RoomData)
{
	const TSoftObjectPtr<UWorld> Level = RoomData->RoomLevel;
	const FString LevelPath = Level.ToSoftObjectPath().GetLongPackageName();
	UE_LOG(LogTemp, Log, TEXT("Loading Level by Path: %s"), *LevelPath);

	UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelPath));
	PreviousRoom = CurrentRoom;
	CurrentRoom = RoomData;
	// PrintFloor();
}

FRoomData* URoomManager::NewRoom(const ERoomType RoomType)
{
	FRoomData* NewRoom = new FRoomData();
	NewRoom->RoomType = RoomType;
	if (FloorRoomData.IsEmpty()) return NewRoom;
	NewRoom->RoomLevel = FloorRoomData[CurrentFloorIndex]->GetRandomRoomLevel(RoomType);
	return NewRoom;
}

void URoomManager::PrintFloor() const
{
	if (!GEngine) return; // GEngine이 null이라면 종료

	FString EntireFloorMap; // 화면 출력을 위한 전체 Floor Map 문자열
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("========== Debug Floor Map =========="));
	EntireFloorMap.Append("========== Debug Floor Map ==========\n");

	for (const TArray<FRoomData*>& RoomRow : Map)
	{
		FString RowDebugString; 
		for (const FRoomData* Room : RoomRow)
		{
			FString RoomInfo;
			if (Room->RoomPoint == CurrentRoom->RoomPoint)
				RoomInfo = FString::Printf(TEXT("O"));
			else if (Room->RoomType == ERoomType::ENone)
				RoomInfo = FString::Printf(TEXT("X"));
			else
				RoomInfo = FString::Printf(TEXT("%d"), static_cast<int32>(Room->RoomType));

			RowDebugString += RoomInfo + TEXT("\t");
		}
		// Console 로그 출력
		UE_LOG(LogTemp, Log, TEXT("%s"), *RowDebugString);

		// 화면 디버그 메시지에 추가
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, RowDebugString);
		EntireFloorMap.Append(RowDebugString + "\n");
	}
}