#include "Monster/Component/ChargeAttackComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Animation/AnimInstance.h"

UChargeAttackComponent::UChargeAttackComponent()
{
    PrimaryComponentTick.bCanEverTick = true;  // Tick 활성화
    bIsCharging = false;
    ChargeStartTime = 0.0f;
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
    PlaySkillMontage();  // 기존 charge 애니메이션 재생
    StartCharge();
}

void UChargeAttackComponent::StartCharge()
{
    ACharacter* OwnerMonster = Cast<ACharacter>(GetOwner());
    if (OwnerMonster)
    {
        // 돌진 시작 시 속도 설정
        OwnerMonster->GetCharacterMovement()->MaxWalkSpeed = ChargeSpeed;

        bIsCharging = true;
        ChargeStartTime = GetWorld()->GetTimeSeconds();

        // 일정 시간 후 돌진 중지
        GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle, this, &UChargeAttackComponent::StopCharge, ChargeDuration, false);
    }
}

void UChargeAttackComponent::StopCharge()
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
        OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 400.0f;
    }

    // 애니메이션 끝내기 (차지 종료 후 종료 애니메이션 재생)
    PlayEndAnimation();

    bIsCharging = false;
}

void UChargeAttackComponent::PlayEndAnimation()
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
        if (AnimInstance && ChargeSkillEndMontage)  // EndAnimationMontage는 에디터에서 설정한 종료 애니메이션 몽타주
        {
            AnimInstance->Montage_Play(ChargeSkillEndMontage);
        }
    }
}

void UChargeAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsCharging)
    {
        ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
        if (OwnerCharacter)
        {
            // 돌진 중 계속 이동 처리
            FVector ForwardDirection = OwnerCharacter->GetActorForwardVector();
            OwnerCharacter->AddMovementInput(ForwardDirection, 1.0f);
        }
    }
}

void UChargeAttackComponent::OnHit(AActor* HitActor)
{
    if (HitActor)
    {
        UGameplayStatics::ApplyDamage(HitActor, Damage, nullptr, GetOwner(), nullptr);
        StopCharge();  // 충돌 시 돌진 중지
    }
}