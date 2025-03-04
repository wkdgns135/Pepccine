// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FloorRoomData.generated.h"

enum class ERoomType : uint8;

UCLASS()
class PEPCCINE_API UFloorRoomData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "6", ToolTip = "맵의 크기는 3의 배수가 권장됩니다."), DisplayName="맵 크기")
	int32 MapSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "4", ToolTip = "맵의 복잡도는 반드시 (MapSize * MapSize) / 9 이하여야 합니다."), DisplayName= "맵 복잡도")
	int32 EndPointCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta = (displayname = "기본 방 레벨 (4Way)"))
	TArray<TSoftObjectPtr<UWorld>> Default4WayRoomLevels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta = (displayname = "기본 방 레벨 (3Way)"))
	TArray<TSoftObjectPtr<UWorld>> Default3WayRoomLevels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta = (displayname = "기본 방 레벨 (2Way 수직 문 구조)"))
	TArray<TSoftObjectPtr<UWorld>> Default2WayPerpendicularRoomLevels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta = (displayname = "기본 방 레벨 (2Way 평행 문 구조)"))
	TArray<TSoftObjectPtr<UWorld>> Default2WayParallelRoomLevels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta = (displayname = "기본 방 레벨 (1Way)"))
	TArray<TSoftObjectPtr<UWorld>> Default1WayRoomLevels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta = (displayname = "시작 방 레벨"))
	TArray<TSoftObjectPtr<UWorld>> StartRoomLevels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta = (displayname = "보스 방 레벨"))
	TArray<TSoftObjectPtr<UWorld>> BossRoomLevels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta = (displayname = "아이템 방 레벨"))
	TArray<TSoftObjectPtr<UWorld>> ItemRoomLevels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta = (displayname = "상점 방 레벨"))
	TArray<TSoftObjectPtr<UWorld>> ShopRoomLevels;
	
public:
	UFloorRoomData();
	TSoftObjectPtr<UWorld> GetRandomRoomLevel(const ERoomType RoomType);
	
public:
	FORCEINLINE TSoftObjectPtr<UWorld> GetRandomDefault4WayRoomLevels(){return Default4WayRoomLevels[FMath::RandRange(0, Default4WayRoomLevels.Num() - 1)];}
	FORCEINLINE TSoftObjectPtr<UWorld> GetRandomDefault3WayRoomLevels(){return Default3WayRoomLevels[FMath::RandRange(0, Default3WayRoomLevels.Num() - 1)];}
	FORCEINLINE TSoftObjectPtr<UWorld> GetRandomDefault2WayPerpendicularRoomLevels(){return Default2WayPerpendicularRoomLevels[FMath::RandRange(0, Default2WayPerpendicularRoomLevels.Num() - 1)];}
	FORCEINLINE TSoftObjectPtr<UWorld> GetRandomDefault2WayParallelRoomLevels(){return Default2WayParallelRoomLevels[FMath::RandRange(0, Default2WayParallelRoomLevels.Num() - 1)];}
	FORCEINLINE TSoftObjectPtr<UWorld> GetRandomDefault1WayRoomLevels(){return Default1WayRoomLevels[FMath::RandRange(0, Default1WayRoomLevels.Num() - 1)];}
};
