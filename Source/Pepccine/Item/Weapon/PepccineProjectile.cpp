#include "Item/Weapon/PepccineProjectile.h"

#include "PepccineCharacter.h"
#include "Character/Components/BattleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ObjectPool/PepccinePoolSubSystem.h"

APepccineProjectile::APepccineProjectile()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &APepccineProjectile::OnHit);

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovementComp->UpdatedComponent = CollisionComp;
	ProjectileMovementComp->InitialSpeed = 3000.f;
	ProjectileMovementComp->MaxSpeed = 3000.f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bShouldBounce = true;
	// InitialLifeSpan = 3.0f;
}

void APepccineProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (ProjectileMovementComp->IsActive())
	{
		 const float CurrentDistance = FVector::Dist(StartLocation, GetActorLocation());

		// 최대 사거리까지 이동 후
		if (CurrentDistance >= MaxDistance)
		{
			// 풀로 돌려보내기
			OnReturnToPool();
		}
	}
}

void APepccineProjectile::InitProjectile(const FVector& ShootDirection, const int32 Speed, const float InMaxDistance)
{
	ProjectileMovementComp->InitialSpeed = Speed;
	ProjectileMovementComp->MaxSpeed = Speed;
	ProjectileMovementComp->Velocity = ShootDirection * ProjectileMovementComp->InitialSpeed;
	MaxDistance = InMaxDistance;
}

void APepccineProjectile::OnSpawnFromPool()
{
	Super::OnSpawnFromPool();
	SetActorTickEnabled(true);
	StartLocation = GetActorLocation();
	ProjectileMovementComp->SetActive(true);
}

void APepccineProjectile::OnReturnToPool()
{
	Super::OnReturnToPool();
	SetActorTickEnabled(false);
	ProjectileMovementComp->SetActive(false);
}

void APepccineProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr && OtherActor != this)
	{
		if (OwnerCharacter)
		{
			if (ACharacter* Enemy = Cast<ACharacter>(OtherActor))
			{
				if (const UBattleComponent* BattleComp = OwnerCharacter->GetComponentByClass<UBattleComponent>())
				{
					BattleComp->SendHitResult(Enemy, WeaponDamage, Hit);
				}
			}
		}

		// UE_LOG(LogTemp, Warning, TEXT("Projectile Destroy!"));

		// 오브젝트 풀로 돌려보내기
		GetWorld()->GetSubsystem<UPepccinePoolSubSystem>()->ReturnToPool(this);
	}
}
