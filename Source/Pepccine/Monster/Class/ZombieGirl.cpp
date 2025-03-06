#include "Monster/Class/ZombieGirl.h"
#include "Monster/AI/Controller/NormalMonsterAIC.h"

AZombieGirl::AZombieGirl()
{
	AIControllerClass = ANormalMonsterAIC::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	MonsterClass = EMonsterClass::Normal;
	MonsterType = EMonsterType::ShortRange;
}