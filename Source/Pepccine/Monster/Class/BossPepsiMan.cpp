#include "Monster/Class/BossPepsiMan.h"
#include "Monster/Component/JumpAttackComponent.h"
#include "Monster/AI/Controller/BossMonsterAIC.h"

ABossPepsiMan::ABossPepsiMan()
{
	AIControllerClass = ABossMonsterAIC::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	MonsterClass = EMonsterClass::Boss;
	MonsterType = EMonsterType::ShortRange;

	JumpSkillComponent = CreateDefaultSubobject<UJumpAttackComponent>(TEXT("JumpAttackComponent"));
}