#pragma once

#include "CoreMinimal.h"
#include "Monster/Class/BossMonster.h"
#include "BossGirlScout.generated.h"

class UJumpAttackComponent;

UCLASS()
class PEPCCINE_API ABossGirlScout : public ABossMonster
{
	GENERATED_BODY()

public:
	ABossGirlScout();
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill|Jump")
	UJumpAttackComponent* JumpSkillComponent;
};
