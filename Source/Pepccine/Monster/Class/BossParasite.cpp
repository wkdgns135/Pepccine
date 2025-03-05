#include "Monster/Class/BossParasite.h"
#include "Monster/Component/SpawnSkillComponent.h"
#include "Monster/AI/Controller/BossParasiteAIC.h"

ABossParasite::ABossParasite()
{
	AIControllerClass = ABossParasiteAIC::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	/*SpawnSkillComponent = CreateDefaultSubobject<USpawnSkillComponent>(TEXT("SpawnSkillComponent"));*/
}