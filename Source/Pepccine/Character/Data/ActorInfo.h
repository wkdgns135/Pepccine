#pragma once

#include "CoreMinimal.h"
#include "ActorInfo.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FDetectedActorInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	AActor* Actor;

	UPROPERTY(BlueprintReadWrite)
	float Distance;
};

USTRUCT(Atomic, BlueprintType)
struct FDetectedActorList
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FDetectedActorInfo> DetectedActors;
};