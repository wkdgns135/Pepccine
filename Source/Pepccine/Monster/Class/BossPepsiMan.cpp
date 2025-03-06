#include "Monster/Class/BossPepsiMan.h"
#include "Monster/AI/Controller/BossMonsterAIC.h"

ABossPepsiMan::ABossPepsiMan()
{
	AIControllerClass = ABossMonsterAIC::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}