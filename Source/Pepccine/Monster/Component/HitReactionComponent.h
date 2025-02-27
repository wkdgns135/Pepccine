#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HitReactionComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PEPCCINE_API UHitReactionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHitReactionComponent();

    void HandleHitReaction(float DamageAmount);
protected:
    virtual void BeginPlay() override;

    /** 최대 피격 카운트 */
    UPROPERTY(EditAnywhere, Category = "Hit Reaction")
    int32 MaxHitCount = 3;  // 예시로 3번

    /** 경직(Stagger) 시간 */
    UPROPERTY(EditAnywhere, Category = "Hit Reaction")
    float StaggerTime = 0.5f;

    /** 넉백(Knockback) 강도 */
    UPROPERTY(EditAnywhere, Category = "Hit Reaction")
    float KnockbackStrength = 500.0f;

    /** 피격 애니메이션 */
    UPROPERTY(EditAnywhere, Category = "Hit Reaction")
    UAnimMontage* HitReactionMontage;

private:
    /** 피격 카운트 */
    int32 HitCount = 0;
};
