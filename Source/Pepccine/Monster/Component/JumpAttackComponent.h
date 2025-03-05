#pragma once

#include "CoreMinimal.h"
#include "Monster/Component/BaseSkillComponent.h"
#include "JumpAttackComponent.generated.h"

UCLASS()
class PEPCCINE_API UJumpAttackComponent : public UBaseSkillComponent
{
	GENERATED_BODY()

public:
	UJumpAttackComponent();

	virtual void ActivateSkill() override;
	void SkillTrace() override;
	void LaunchMonster();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|JumpAttack")
	float JumpPower = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|JumpAttack")
	float JumpHeight = 800.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|JumpAttack")
	float AttackRadius = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|JumpAttack")
	float DamageMultiplier = 1.2f;

	virtual void BeginPlay() override;
private:
	void SetSkillDamage();
	float SkillDamage;
};
