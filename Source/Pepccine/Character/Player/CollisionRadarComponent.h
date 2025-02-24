// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "CollisionRadarComponent.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FDetectedActorList
{
  GENERATED_BODY()

  UPROPERTY()
  TArray<AActor*> DetectedActors;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorDetectedEnhanced, const FDetectedActorList&, DetectedActors);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEPCCINE_API UCollisionRadarComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCollisionRadarComponent();

  UPROPERTY(BlueprintAssignable, Category = "Radar|Events")
  FOnActorDetectedEnhanced OnActorDetectedEnhanced;

  // Debug 용
  UPROPERTY(EditAnywhere, Category = "Debug")
  bool bShowDetectionRadius = false;

  UPROPERTY(EditAnywhere, Category = "Debug")
  bool bShowFieldOfView = false; // FOV

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
  UPROPERTY(VisibleAnywhere, Category = "Radar")
  USphereComponent* DetectionZone; // 감지 반경을 위한 Sphere 콜리전

  UPROPERTY(EditAnywhere, Category = "Radar|Settings")
  float DetectionRadius = 1200.0f; // 감지 거리

  UPROPERTY(EditAnywhere, Category = "Radar|Settings")
  float FieldOfView = 120.0f; // 시야각 (FOV)

  UPROPERTY(EditAnywhere, Category = "Debug")
  bool bShowDebug = true; // 디버그 모드

  TArray<AActor*> NearbyActors; // 감지 범위 내 액터 리스트

  bool IsInFieldOfView(AActor* TargetActor) const;
  void DetectActors();
  void DrawDebugVisualization();
  void AddDetectZone();

  UFUNCTION()
  void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult);

  UFUNCTION()
  void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
