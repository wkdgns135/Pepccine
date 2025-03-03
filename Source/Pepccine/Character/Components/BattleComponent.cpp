#include "BattleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"

UBattleComponent::UBattleComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBattleComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBattleComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBattleComponent::SendHitResult(AActor* HitTarget, float DamageAmount, const FHitResult& HitResult, EMonsterSkill SkillType) const
{
	if (HitTarget)
	{
		if (UBattleComponent* TargetBattleComponent = HitTarget->FindComponentByClass<UBattleComponent>())
		{
			UE_LOG(LogTemp, Warning, TEXT("Sending Hit: %s -> %s (Damage: %f)"), *GetOwner()->GetName(), *HitTarget->GetName(), DamageAmount);
			TargetBattleComponent->ReceiveHitResult(DamageAmount, GetOwner(), HitResult, SkillType);
		}
	}
}

void UBattleComponent::ReceiveHitResult(float DamageAmount, AActor* DamageCauser, const FHitResult& HitResult, EMonsterSkill SkillType) const
{
	if (const AActor* OwnerActor = GetOwner())
	{
		OnCharacterHited.Broadcast(DamageCauser, DamageAmount, HitResult, SkillType);
	}
}
