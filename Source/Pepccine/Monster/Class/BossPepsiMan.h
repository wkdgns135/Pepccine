#pragma once

#include "CoreMinimal.h"
#include "Monster/Class/BossMonster.h"
#include "BossPepsiMan.generated.h"

class UJumpAttackComponent;

UCLASS()
class PEPCCINE_API ABossPepsiMan : public ABossMonster
{
	GENERATED_BODY()
	
public:
	ABossPepsiMan();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill|Jump")
	UJumpAttackComponent* JumpSkillComponent;
};
