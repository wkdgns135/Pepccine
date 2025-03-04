#include "Character/Components/CollisionRadarComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h" // Debug
#include "Character/Data/ActorInfo.h"
#include "Item/PepccineDropItem.h"

/*
Collision Enabled 된 대상만 가능(벽넘기는 별개)
*/

UCollisionRadarComponent::UCollisionRadarComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	DetectionZone = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionZone"));
	DetectionZone->InitSphereRadius(DetectionRadius);
}

void UCollisionRadarComponent::BeginPlay()
{
	Super::BeginPlay();

	if (DetectionZone)
	{
		DetectionZone->SetCollisionProfileName(TEXT("OverlapAll"));
	}

	if (!bIsUseRadar || !DetectionZone) return;
	
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

void UCollisionRadarComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (AActor* Owner = GetOwner())
	{
		DetectActors();
		DrawDebugVisualization();
	}

	if (bIsUseSweep)
	{
		bIsAbleToClimb = CanClimbOverObstacle();
	}
}

bool UCollisionRadarComponent::CanClimbOverObstacle() const
{
	AActor* Owner = GetOwner();
	if (!Owner) return false;

	FVector Start = Owner->GetActorLocation();
	FVector End = Start + (Owner->GetActorForwardVector() * 100.f);

	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	bool bHit = GetWorld()->SweepMultiByObjectType(
			HitResults,
			Start,
			End,
			FQuat::Identity,
			ObjectQueryParams,
			FCollisionShape::MakeCapsule(34.f, 88.f),
			QueryParams
	);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			if (Hit.bBlockingHit)
			{
				if (bShowDebug)
				{
					DrawDebugCapsule(GetWorld(), Hit.ImpactPoint, 100.0f, 50.0f, FQuat::Identity, FColor::Red, false, 2.0f);
				}
				
				float ObstacleTop = Hit.GetActor()->GetActorLocation().Z + Hit.GetActor()->GetSimpleCollisionHalfHeight();
				float CharacterBottom = Start.Z;
				float ObstacleHeight = ObstacleTop - CharacterBottom;
				
				FVector ObstacleExtent = Hit.GetActor()->GetComponentsBoundingBox().GetExtent();
				float ObstacleWidth = ObstacleExtent.X * 2;
				//UE_LOG(LogTemp, Warning, TEXT("벽 높이: [%f], 벽 폭: [%f]"), ObstacleHeight, ObstacleWidth);
				
				if (ObstacleHeight < 110 && ObstacleWidth < 150)
				{
					//UE_LOG(LogTemp, Warning, TEXT("벽을 넘을 수 있음!"));
					return true;
				}
			}
		}
	}

	return false;
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
	//UE_LOG(LogTemp, Warning, TEXT("OnOverlapBegin -> [%s]"), *OtherActor->GetName());

	if (OtherActor && OtherActor != GetOwner())
	{
		NearbyActors.AddUnique(OtherActor);
	}
}

void UCollisionRadarComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("End -> [%s]"), *OtherActor->GetName());
	if (OtherActor && OtherActor != GetOwner())
	{
		if (const APepccineDropItem* Item = Cast<APepccineDropItem>(OtherActor))
		{
			Item->ShowInteractWidget(false);
		}
		NearbyActors.Remove(OtherActor);
	}
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

		const float Distance = FVector::Dist(GetOwner()->GetActorLocation(), Actor->GetActorLocation());

		if (IsInFieldOfView(Actor))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Detected -> [%s]"), *Actor->GetName());
			FDetectedActorInfo ActorInfo;
			ActorInfo.Actor = Actor;
			ActorInfo.Distance = Distance;

			DetectedActorList.DetectedActors.Add(ActorInfo);
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
		DrawDebugCone(GetWorld(), Location, Forward, DetectionRadius, HalfFOV, HalfFOV, 30, FColor::Red, false, 0.1f, 0,
		              2.0f);
	}
}
