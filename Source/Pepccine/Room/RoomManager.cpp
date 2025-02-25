// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomManager.h"

#include "FloorRoomData.h"

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
			default: break;
			case 1 : Map[i][j] = NewRoom(ERoomType::EDefault); break;
			case 2 : Map[i][j] = NewRoom(ERoomType::EStart); break;
			case 3 : Map[i][j] = NewRoom(ERoomType::EBoss); break;
			case 4 : Map[i][j] = NewRoom(ERoomType::EItem); break;
			case 5 : Map[i][j] = NewRoom(ERoomType::EShop); break;
			}
			
			if (Map[i][j].RoomType == ERoomType::EStart)
			{
				CurrentRoom = {j ,i};
			}
		}
	}
}

void URoomManager::StartFloor()
{
	const TSoftObjectPtr<UWorld> StartLevel = GetCurrentRoomLevel();
		
	// 전체 경로 가져오기
	const FString LevelPath = StartLevel.ToSoftObjectPath().GetLongPackageName();
	UE_LOG(LogTemp, Log, TEXT("Loading Level by Path: %s"), *LevelPath);

	// 정확한 패키지 경로를 사용해 레벨 로드
	UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelPath));
}

FRoomData URoomManager::NewRoom(const ERoomType RoomType)
{
	FRoomData NewRoom;
	NewRoom.RoomType = RoomType;
	if (FloorRoomData.IsEmpty()) return NewRoom;
	NewRoom.RoomLevel = FloorRoomData[CurrentFloorIndex]->GetRandomRoomLevel(RoomType);
	return NewRoom;
}
