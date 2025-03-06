#include "Monster/Class/BossPumpkinHulk.h"
#include "Monster/Component/JumpAttackComponent.h"
#include "Monster/Component/ChargeAttackComponent.h"
#include "Monster/AI/Controller/BossPumpkinHulkAIC.h"

ABossPumpkinHulk::ABossPumpkinHulk()
{
	AIControllerClass = ABossPumpkinHulkAIC::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	MonsterClass = EMonsterClass::Boss;
	MonsterType = EMonsterType::ShortRange;

	JumpSkillComponent = CreateDefaultSubobject<UJumpAttackComponent>(TEXT("JumpAttackComponent"));
	ChargeSkillComponent = CreateDefaultSubobject<UChargeAttackComponent>(TEXT("ChargeAttackComponent"));
}