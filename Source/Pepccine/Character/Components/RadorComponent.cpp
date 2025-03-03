#include "Character/Components/RadorComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Player/PepCharacter.h"
#include "DrawDebugHelpers.h"

URadorComponent::URadorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URadorComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void URadorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  if (AActor* Owner = GetOwner())
  {
    DetectActors();
    DrawDebugLine();
  }
}

void URadorComponent::DrawDebugLine()
{
  if (!bShowDetectionRadius && !bShowFieldOfView) return;

  FVector Location = GetOwner()->GetActorLocation();
  FVector Forward = GetOwner()->GetActorForwardVector();

  if (bShowDetectionRadius)
  {
    DrawDebugSphere(GetWorld(), Location, DetectionRadius, 32, FColor::Green, false, 0.1f, 0, 2.0f);
  }

  if (bShowFieldOfView)
  {
    float HalfFOV = FMath::DegreesToRadians(FieldOfView * 0.5f);
    DrawDebugCone(GetWorld(), Location, Forward, DetectionRadius, HalfFOV, HalfFOV, 30, FColor::Red, false, 0.1f, 0, 2.0f);
  }
}

void URadorComponent::DetectActors()
{
  TArray<AActor*> DetectedActors;
  UGameplayStatics::GetAllActorsOfClass(GetWorld(), DetectionClass, DetectedActors);

  for (AActor* Actor : DetectedActors)
  {
    if (!Actor || Actor == GetOwner()) continue;

    float Distance = FVector::Dist(GetOwner()->GetActorLocation(), Actor->GetActorLocation());
    if (Distance > DetectionRadius) continue;

    if (IsInFieldOfView(Actor))
    {
      OnActorDetected.Broadcast(Actor);
    }
  }
}

bool URadorComponent::IsInFieldOfView(AActor* TargetActor) const
{
  if (!TargetActor) return false;

  FVector MyLocation = GetOwner()->GetActorLocation();
  FVector TargetLocation = TargetActor->GetActorLocation();

  FVector ForwardVector = GetOwner()->GetActorForwardVector().GetSafeNormal();

  FVector ToTarget = (TargetLocation - MyLocation).GetSafeNormal();

  float DotProduct = FVector::DotProduct(ForwardVector, ToTarget);

  float CosHalfFOV = FMath::Cos(FMath::DegreesToRadians(FieldOfView * 0.5f));

  return DotProduct >= CosHalfFOV;
}