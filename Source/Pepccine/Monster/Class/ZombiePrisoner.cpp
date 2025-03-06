#include "Monster/Class/ZombiePrisoner.h"
#include "Monster/AI/Controller/NormalMonsterAIC.h"

AZombiePrisoner::AZombiePrisoner()
{
	AIControllerClass = ANormalMonsterAIC::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	MonsterClass = EMonsterClass::Normal;
	MonsterType = EMonsterType::ShortRange;

}