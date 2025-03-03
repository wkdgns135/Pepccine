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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    UAnimMontage* ChargeSkillEndMontage;
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY(EditAnywhere, Category = "Skill")
    float ChargeSpeed = 1000.0f;  // 돌진 속도
    UPROPERTY(EditAnywhere, Category = "Skill")
    float ChargeDuration = 3.0f;  // 돌진 지속 시간

    
    void StartCharge();
    void StopCharge();
    void PlayEndAnimation();
    void OnHit(AActor* HitActor);

    FTimerHandle ChargeTimerHandle;

    bool bIsCharging;  // Charging 상태 추적
    float ChargeStartTime;  // Charge 시작 시간 추적
};
