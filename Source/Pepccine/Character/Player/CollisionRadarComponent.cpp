#include "Character/Player/CollisionRadarComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h" // Debug

/*
Collision Enabled 된 대상만 가능
*/

UCollisionRadarComponent::UCollisionRadarComponent()
{
  PrimaryComponentTick.bCanEverTick = true;

  DetectionZone = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionZone"));
  DetectionZone->InitSphereRadius(DetectionRadius);
  DetectionZone->SetCollisionProfileName(TEXT("OverlapAll"));
}

void UCollisionRadarComponent::BeginPlay()
{
	Super::BeginPlay();

  if (!bIsUseRadar) return;
  if (!DetectionZone) return;
  AddDetectZone();

  if (AActor* Owner = GetOwner())
  {
    if (!Owner->GetRootComponent())
    {
      Owner->SetRootComponent(DetectionZone);
    }
    else
    {
      DetectionZone->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    }
  }
}

void UCollisionRadarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  if (AActor* Owner = GetOwner())
  {
    DetectActors();
    DrawDebugVisualization();
  }
}

void UCollisionRadarComponent::AddDetectZone()
{
  DetectionZone->OnComponentBeginOverlap.AddDynamic(this, &UCollisionRadarComponent::OnOverlapBegin);
  DetectionZone->OnComponentEndOverlap.AddDynamic(this, &UCollisionRadarComponent::OnOverlapEnd);
}

void UCollisionRadarComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
  bool bFromSweep, const FHitResult& SweepResult)
{
  if (OtherActor && OtherActor != GetOwner())
  {
    NearbyActors.AddUnique(OtherActor);
  }
}

void UCollisionRadarComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  NearbyActors.Remove(OtherActor);
}

bool UCollisionRadarComponent::IsInFieldOfView(AActor* TargetActor) const
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

void UCollisionRadarComponent::DetectActors()
{
  FDetectedActorList DetectedActorList;

  for (AActor* Actor : NearbyActors)
  {
    if (!Actor) continue;

    if (IsInFieldOfView(Actor))
    {
      DetectedActorList.DetectedActors.Add(Actor);
    }
  }

  if (DetectedActorList.DetectedActors.Num() > 0)
  {
    OnActorDetectedEnhanced.Broadcast(DetectedActorList);
  }
}

void UCollisionRadarComponent::DrawDebugVisualization()
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
