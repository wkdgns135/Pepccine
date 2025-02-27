#include "Monster/Component/BaseSkillComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"

UBaseSkillComponent::UBaseSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

    LastUsedTime = 0; // 처음엔 쿨타임이 적용되도록 0으로 설정
}

void UBaseSkillComponent::BeginPlay()
{
	Super::BeginPlay();
}


bool UBaseSkillComponent::IsCooldownOver() const
{
    return (GetWorld()->GetTimeSeconds() - LastUsedTime) >= CooldownTime;
}

void UBaseSkillComponent::StartCooldown()
{
    LastUsedTime = GetWorld()->GetTimeSeconds();
}

void UBaseSkillComponent::PlaySkillMontage()
{
    if (SkillMontage && GetOwner())
    {
        AActor* Owner = GetOwner();
        USkeletalMeshComponent* MeshComp = Owner->FindComponentByClass<USkeletalMeshComponent>();

        if (MeshComp && MeshComp->GetAnimInstance())
        {
            UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
            AnimInstance->Montage_Play(SkillMontage);
        }
    }
}

void UBaseSkillComponent::ApplyDamageToTarget(AActor* Target)
{
    if (Target)
    {
        // 예시 코드: 타겟에 데미지를 주는 로직 (주석 처리)
        /*
        float DamageAmount = 20.0f; // 예시 데미지 수치
        UGameplayStatics::ApplyDamage(Target, DamageAmount, nullptr, GetOwner(), nullptr);
        */
    }
}