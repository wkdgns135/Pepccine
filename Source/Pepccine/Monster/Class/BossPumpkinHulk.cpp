#include "Monster/Class/BossPumpkinHulk.h"
#include "Monster/Component/JumpAttackComponent.h"
#include "Monster/Component/ChargeAttackComponent.h"
#include "Monster/AI/Controller/BossPumpkinHulkAIC.h"

ABossPumpkinHulk::ABossPumpkinHulk()
{
	AIControllerClass = ABossPumpkinHulkAIC::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	JumpSkillComponent = CreateDefaultSubobject<UJumpAttackComponent>(TEXT("JumpAttackComponent"));
	ChargeSkillComponent = CreateDefaultSubobject<UChargeAttackComponent>(TEXT("ChargeAttackComponent"));
}