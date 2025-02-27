#pragma once

#include "CoreMinimal.h"
#include "Monster/Class/BossMonster.h"
#include "BossPumpkinHulk.generated.h"

class UJumpAttackComponent;
class UChargeAttackComponent;

UCLASS()
class PEPCCINE_API ABossPumpkinHulk : public ABossMonster
{
	GENERATED_BODY()
public:
	ABossPumpkinHulk();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill|Jump")
	UJumpAttackComponent* JumpSkillComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill|Charge")
	UChargeAttackComponent* ChargeSkillComponent;
};