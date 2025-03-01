#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectPool/PepccinePoolable.h"

#include "PepccineProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(Abstract)
class PEPCCINE_API APepccineProjectile : public APepccinePoolable
{
	GENERATED_BODY()

public:
	APepccineProjectile();

	// 이동방향 및 속도 초기화
	void InitProjectile(const FVector& ShootDirection, const int32 Speed) const;

	virtual void OnSpawnFromPool() override;
	virtual void OnReturnToPool() override;
	
	// 프로젝타일 충돌 이벤트
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	           const FHitResult& Hit);

	// getter
	FORCEINLINE USphereComponent* GetCollisionComp() const { return CollisionComp; }
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovementComp; }

	// setter
	FORCEINLINE void SetOwnerCharacter(ACharacter* Character) { OwnerCharacter = Character; };
	FORCEINLINE void SetWeaponDamage(const float& InWeaponDamage) { WeaponDamage = InWeaponDamage; };

protected:
	// 충돌 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	USphereComponent* CollisionComp;

	// 프로젝타일 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovementComp;

	// 무기가 부착된 캐릭터
	UPROPERTY()
	ACharacter* OwnerCharacter;

	// 무기 공격력
	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	float WeaponDamage = 0.0f;
};
