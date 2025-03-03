#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "CollisionRadarComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorDetectedEnhanced, FDetectedActorList&, DetectedActors);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEPCCINE_API UCollisionRadarComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCollisionRadarComponent();

  UPROPERTY(BlueprintAssignable, Category = "Radar|Events")
  FOnActorDetectedEnhanced OnActorDetectedEnhanced;

  UPROPERTY(EditAnywhere, Category = "Debug")
  bool bShowDetectionRadius = false;

  UPROPERTY(EditAnywhere, Category = "Debug")
  bool bShowFieldOfView = false; // FOV

  UPROPERTY(EditAnywhere, Category = "Radar")
  bool bIsUseRadar = false;

	UPROPERTY(EditAnywhere, Category = "Radar")
	bool bIsUseSweep = false;

	FORCEINLINE_DEBUGGABLE bool IsAbleToClimb() const { return bIsAbleToClimb; } 
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
  UPROPERTY(VisibleAnywhere, Category = "Radar")
  USphereComponent* DetectionZone;

  UPROPERTY(EditAnywhere, Category = "Radar|Settings")
  float DetectionRadius = 1200.0f;

  UPROPERTY(EditAnywhere, Category = "Radar|Settings")
  float FieldOfView = 120.0f;

  UPROPERTY(EditAnywhere, Category = "Debug")
  bool bShowDebug = true;

	bool bIsAbleToClimb = false;

	UPROPERTY()
  TArray<AActor*> NearbyActors;

  bool IsInFieldOfView(AActor* TargetActor) const;
  void DetectActors();
  void DrawDebugVisualization();
  void AddDetectZone();
	bool CanClimbOverObstacle() const;

  UFUNCTION()
  void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult);

  UFUNCTION()
  void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
