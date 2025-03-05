#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseSkillComponent.generated.h"

UCLASS(Abstract)
class PEPCCINE_API UBaseSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
    virtual void ActivateSkill() PURE_VIRTUAL(UBaseSkillComponent::ActivateSkill, );
 
    virtual void SkillTrace() {};

    UBaseSkillComponent();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FString SkillName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float CooldownTime;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    UAnimMontage* SkillMontage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float Damage;
    

    void PlaySkillMontage();
    bool IsCooldownOver() const;
    void StartCooldown();

	virtual void BeginPlay() override;

private:

    void ApplyDamageToTarget(AActor* Target);

    float LastUsedTime;
};
