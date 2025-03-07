#include "Monster/Class/BossWarZombie.h"
#include "Monster/Component/StealthModeSkillComponent.h"
#include "Monster/AI/Controller/BossWarZombieAIC.h"

ABossWarZombie::ABossWarZombie()
{
	AIControllerClass = ABossWarZombieAIC::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	MonsterType = EMonsterType::LongRange;
	//StealthModeComponent = CreateDefaultSubobject<UStealthModeSkillComponent>(TEXT("StealthModeComponent"));
}
