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

void UBattleComponent::SendHitResult(AActor* HitTarget, float DamageAmount, FHitResult HitResult)
{
	if (HitTarget)
	{
		UBattleComponent* TargetBattleComponent = HitTarget->FindComponentByClass<UBattleComponent>();

		if (TargetBattleComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("Sending Hit: %s -> %s (Damage: %f)"), *GetOwner()->GetName(), *HitTarget->GetName(), DamageAmount);
            
			TargetBattleComponent->ReceiveHitResult(DamageAmount, GetOwner(), HitResult);
		}
	}
}

void UBattleComponent::ReceiveHitResult(float DamageAmount, AActor* DamageCauser, FHitResult HitResult)
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		OnCharacterHited.Broadcast(DamageCauser, HitResult);
	}
}