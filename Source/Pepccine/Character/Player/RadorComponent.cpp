#include "Character/Player/RadorComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "PepCharacter.h"
#include "DrawDebugHelpers.h" // 디버그용 헤더

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

  // 감지반경
  if (bShowDetectionRadius)
  {
    DrawDebugSphere(GetWorld(), Location, DetectionRadius, 32, FColor::Green, false, 0.1f, 0, 2.0f);
  }

  // 시야각
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

  // Tag 아이템이나 몬스터

  for (AActor* Actor : DetectedActors)
  {
    if (!Actor || Actor == GetOwner()) continue;

    // 감지 반경 내에 있는지 확인
    float Distance = FVector::Dist(GetOwner()->GetActorLocation(), Actor->GetActorLocation());
    if (Distance > DetectionRadius) continue;

    // 시야각 내에 있는지 확인
    if (IsInFieldOfView(Actor))
    {
      OnActorDetected.Broadcast(Actor); // 감지 이벤트 호출
    }
  }
}

// 내적(Dot Product): 두 벡터가 특정 각도 안에 있는지 판단할 때 사용 (FOV 체크)
// 외적(Cross Product) : 벡터 간의 수직 관계를 확인할 때 사용
bool URadorComponent::IsInFieldOfView(AActor* TargetActor) const
{
  if (!TargetActor) return false;

  FVector MyLocation = GetOwner()->GetActorLocation();
  FVector TargetLocation = TargetActor->GetActorLocation();

  // 내 앞을 바라보는 벡터
  FVector ForwardVector = GetOwner()->GetActorForwardVector().GetSafeNormal();

  // 목표 방향 벡터
  FVector ToTarget = (TargetLocation - MyLocation).GetSafeNormal();

  // 내적(dot product) 계산
  float DotProduct = FVector::DotProduct(ForwardVector, ToTarget);

  // 시야각 코사인 값
  float CosHalfFOV = FMath::Cos(FMath::DegreesToRadians(FieldOfView * 0.5f));

  return DotProduct >= CosHalfFOV;
}