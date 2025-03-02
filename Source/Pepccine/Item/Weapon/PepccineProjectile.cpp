#include "Item/Weapon/PepccineProjectile.h"

#include "PepccineCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

APepccineProjectile::APepccineProjectile()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &APepccineProjectile::OnHit);

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	InitialLifeSpan = 3.0f;
}

void APepccineProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr && OtherActor != this)
	{
		//OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		if (OwnerCharacter)
		{
			if (ACharacter* Enemy = Cast<ACharacter>(OtherActor))
			{
				UGameplayStatics::ApplyDamage(Enemy, WeaponDamage, OwnerCharacter->GetController(), this,
				                              UDamageType::StaticClass());
			}
		}

		// UE_LOG(LogTemp, Warning, TEXT("Projectile Destroy!"));

		Destroy();
	}
}

void APepccineProjectile::SetProjectileVelocity(const FVector& Direction) const
{
	ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
}
