#pragma once

#include "CoreMinimal.h"
#include "Monster/Component/BaseSkillComponent.h"
#include "GameFramework/Actor.h"
#include "ChargeAttackComponent.generated.h"

UCLASS()
class PEPCCINE_API UChargeAttackComponent : public UBaseSkillComponent
{
    GENERATED_BODY()

public:
    UChargeAttackComponent();

    virtual void ActivateSkill() override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Charge")
    UAnimMontage* ChargeSkillEndMontage;
    UPROPERTY(EditAnywhere, Category = "Skill|Charge")
    float ChargeSpeed = 1000.0f;  // 돌진 속도
    UPROPERTY(EditAnywhere, Category = "Skill|Charge")
    float ChargeDuration = 3.0f;  // 돌진 지속 시간
    UPROPERTY(EditAnywhere, Category = "Skill|Charge")
    float AttackRadius = 50.0f;  // 충돌 판정 범위
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Charge")
    float DamageMultiplier = 1.2f;

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    void SetSkillDamage();
    void StartCharge();
    void StopCharge();
    void PlayEndAnimation();
    void ChargeTrace();

    TSet<AActor*> DamagedActors; // 중복 공격 방지용

    void OnHit(AActor* HitActor, const FHitResult& Hit);


    FTimerHandle ChargeTimerHandle;

    bool bIsCharging;  // Charging 상태 추적
    float ChargeStartTime;  // Charge 시작 시간 추적
    float OriginalSpeed;
    float SkillDamage;
};
