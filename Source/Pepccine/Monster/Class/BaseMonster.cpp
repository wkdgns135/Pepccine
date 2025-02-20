#include "BaseMonster.h"
#include "Components/CapsuleComponent.h"

ABaseMonster::ABaseMonster()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseMonster::OnDeath()
{
	Destroy();
}

void ABaseMonster::OnHit()
{
	/*TakeDamage(amount);*/
}

