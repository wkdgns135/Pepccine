#pragma once

#include "CoreMinimal.h"
#include "Monster/Class/BossMonster.h"
#include "BossWarZombie.generated.h"

class UStealthModeSkillComponent;

UCLASS()
class PEPCCINE_API ABossWarZombie : public ABossMonster
{
	GENERATED_BODY()

public:
	ABossWarZombie();

protected:
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill|Stealth")
	//UStealthModeSkillComponent* StealthModeComponent;
};