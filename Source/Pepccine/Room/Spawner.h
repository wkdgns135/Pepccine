// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class ABaseMonster;

UCLASS()
class PEPCCINE_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawner();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:

private:
	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<ABaseMonster> SpawnMonsterClass;
};
