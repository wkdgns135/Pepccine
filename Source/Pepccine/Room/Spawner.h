// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class ABaseMonster;
class APepccineGameState;

UCLASS()
class PEPCCINE_API ASpawner : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<ABaseMonster> SpawnMonsterClass;
	APepccineGameState* GameState;

public:
	ASpawner();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void SpawnMonster();
};
