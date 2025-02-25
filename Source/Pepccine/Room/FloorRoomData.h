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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta = (displayname = "시작 방 레벨"))
	TArray<TSoftObjectPtr<UWorld>> StartRoomLevels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta = (displayname = "기본 방 레벨"))
	TArray<TSoftObjectPtr<UWorld>> DefaultRoomLevels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta = (displayname = "보스 방 레벨"))
	TArray<TSoftObjectPtr<UWorld>> BossRoomLevels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta = (displayname = "아이템 방 레벨"))
	TArray<TSoftObjectPtr<UWorld>> ItemRoomLevels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room", meta = (displayname = "상점 방 레벨"))
	TArray<TSoftObjectPtr<UWorld>> ShopRoomLevels;
	
public:
	TSoftObjectPtr<UWorld> GetRandomRoomLevel(const ERoomType RoomType);
};
