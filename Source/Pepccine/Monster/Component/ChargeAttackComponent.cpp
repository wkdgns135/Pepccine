#include "Monster/Component/ChargeAttackComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

UChargeAttackComponent::UChargeAttackComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UChargeAttackComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UChargeAttackComponent::ActivateSkill()
{
    if (!IsCooldownOver())
    {
        return;
    }

    StartCooldown();
    PlaySkillMontage();
    StartCharge();
}

void UChargeAttackComponent::StartCharge()
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        // 몬스터의 현재 이동 방향으로 돌진
        FVector ForwardDirection = OwnerCharacter->GetActorForwardVector();

        // 이동 속도 설정 (MaxWalkSpeed가 아닌 CharacterMovementComponent의 AddMovementInput 사용)
        OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = ChargeSpeed;

        // LaunchCharacter를 사용하여 이동
        FVector LaunchVelocity = ForwardDirection * ChargeSpeed;
        OwnerCharacter->LaunchCharacter(LaunchVelocity, true, true);  // 물리적인 힘을 통해 캐릭터 이동

        // 일정 시간 후 돌진 중지
        GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle, this, &UChargeAttackComponent::StopCharge, ChargeDuration, false);
    }
}

void UChargeAttackComponent::StopCharge()
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        // 돌진을 멈추고, 다시 기본 속도로 이동
        OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
        OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 400.0f;  // 기본 이동 속도
    }
}

void UChargeAttackComponent::OnHit(AActor* HitActor)
{
    if (HitActor)
    {
        UGameplayStatics::ApplyDamage(HitActor, ChargeDamage, nullptr, GetOwner(), nullptr);
    }
}
