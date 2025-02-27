#include "Monster/Class/BossGirlScout.h"
#include "Monster/Component/JumpAttackComponent.h"
#include "Monster/AI/Controller/BossGirlScoutAIC.h"

ABossGirlScout::ABossGirlScout()
{
	AIControllerClass = ABossGirlScoutAIC::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	JumpSkillComponent = CreateDefaultSubobject<UJumpAttackComponent>(TEXT("JumpAttackComponent"));
}