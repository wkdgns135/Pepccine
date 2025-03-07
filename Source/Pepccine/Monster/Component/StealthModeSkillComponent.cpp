#include "Monster/Component/StealthModeSkillComponent.h"
#include "Monster/Component/MonsterStatComponent.h"
#include "Monster/Class/BaseMonster.h"
#include "Monster/Class/BossWarZombie.h"
#include "GameFramework/Actor.h"

UStealthModeSkillComponent::UStealthModeSkillComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UStealthModeSkillComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UStealthModeSkillComponent::ActivateSkill()
{
    if (bIsStealthMode || !IsCooldownOver())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cooldown not over yet or already in Stealth Mode"));
        return;
    }

    bIsStealthMode = true;
    StartCooldown();
    PlaySkillMontage();
    BoostSpeed(SpeedMultiplier);

    // 블루프린트에서 머티리얼 Dissolve 적용
    OnStealthActivated();

    if (AActor* Owner = GetOwner())
    {
        if (ABaseMonster* Monster = Cast<ABaseMonster>(Owner))
        {
            Monster->HideHealthBar();
        }
    }


    // StealthTime 후 속도 복구 타이머 설정
    GetWorld()->GetTimerManager().SetTimer(
        StealthTimerHandle,
        this,
        &UStealthModeSkillComponent::ResetSpeed,
        StealthTime,
        false
    );

    UE_LOG(LogTemp, Warning, TEXT("Stealth mode activated. Speed boosted by %.2f for %.2f seconds."), SpeedMultiplier, StealthTime);
}

void UStealthModeSkillComponent::BoostSpeed(float Amount)
{
    if (AActor* Owner = GetOwner())
    {
        if (UMonsterStatComponent* StatComp = Owner->FindComponentByClass<UMonsterStatComponent>())
        {
            StatComp->IncreaseSPD(Amount);

        }
    }
}

void UStealthModeSkillComponent::ResetSpeed()
{
    if (AActor* Owner = GetOwner())
    {
        if (ABaseMonster* Monster = Cast<ABaseMonster>(Owner))
        {
            Monster->ShowHealthBar();
        }

        if (UMonsterStatComponent* StatComp = Owner->FindComponentByClass<UMonsterStatComponent>())
        {
            StatComp->IncreaseSPD(1.0f / SpeedMultiplier);
            UE_LOG(LogTemp, Warning, TEXT("Stealth Mode End"));
        }
    }

    // 블루프린트에서 머티리얼 복원 처리
    OnStealthDeactivated();

    bIsStealthMode = false;
}
