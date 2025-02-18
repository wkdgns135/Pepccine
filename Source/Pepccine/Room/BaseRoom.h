// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseRoom.generated.h"

//class ASpawner;

UCLASS()
class PEPCCINE_API UBaseRoom : public UObject
{
	GENERATED_BODY()
	
public:

protected:

private:
	//UPROPERTY()
	//TArray<ASpawner*> Spawners;
	bool bIsCleared;

public:
	FORCEINLINE bool GetIsCleared() const { return bIsCleared; }
};
