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
	virtual void BeginPlay() override;

private:
    void StartCharge();
    void StopCharge();
    void OnHit(AActor* HitActor);

    UPROPERTY(EditAnywhere, Category = "Skill")
    float ChargeSpeed = 1000.0f;  // 돌진 속도

    UPROPERTY(EditAnywhere, Category = "Skill")
    float ChargeDuration = 3.0f;  // 돌진 지속 시간

    UPROPERTY(EditAnywhere, Category = "Skill")
    float ChargeDamage = 30.0f;

    FTimerHandle ChargeTimerHandle;
};
