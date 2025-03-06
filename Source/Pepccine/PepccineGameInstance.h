// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PepccineGameInstance.generated.h"

class APepccineDropItem;
class UPepccineItemDataAssetBase;
class URoomManager;

UCLASS()
class PEPCCINE_API UPepccineGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "RoomManager")
	URoomManager *RoomManager;

	UPROPERTY(EditAnywhere, Category = "Item")
	UPepccineItemDataAssetBase* ItemDataAsset;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<APepccineDropItem> SpawnActor;
	
public:
	UFUNCTION(BlueprintPure, Category = "RoomManager")
	FORCEINLINE URoomManager* GetRoomManager() const { return RoomManager; }

	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE UPepccineItemDataAssetBase* GetItemDataAsset() const { return ItemDataAsset; }
	UFUNCTION(BlueprintPure, Category = "Item")
	FORCEINLINE TSubclassOf<APepccineDropItem> GetSpawnActor() const { return SpawnActor; }
};
