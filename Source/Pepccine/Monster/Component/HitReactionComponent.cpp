// HitReactionComponent.cpp
#include "Monster/Component/HitReactionComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UHitReactionComponent::UHitReactionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHitReactionComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UHitReactionComponent::HandleHitReaction(float DamageAmount)
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter) return;

    HitCount++;

    // 히트 카운트가 최대값에 도달했을 때만 애니메이션 재생
    if (HitCount >= MaxHitCount)
    {
        if (HitReactionMontage)
        {
            UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
            if (AnimInstance)
            {
                AnimInstance->Montage_Play(HitReactionMontage);
            }
        }

        HitCount = 0;
    }

    // 넉백 적용
    /*ApplyKnockback(HitDirection);*/  // 필요하면 넉백 로직 활성화
}
