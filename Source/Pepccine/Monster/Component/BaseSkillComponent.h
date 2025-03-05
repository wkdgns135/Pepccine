#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseSkillComponent.generated.h"

UENUM(BlueprintType)
enum class ESkillType : uint8
{
    None            UMETA(DisplayName = "None"),
    Passive         UMETA(DisplayName = "Passive"),
    Active    UMETA(DisplayName = "Active"),
};

UCLASS(Abstract)
class PEPCCINE_API UBaseSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
    virtual void ActivateSkill() PURE_VIRTUAL(UBaseSkillComponent::ActivateSkill, );
 
    virtual void SkillTrace() {};

    UBaseSkillComponent();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
    ESkillType SkillType;
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
