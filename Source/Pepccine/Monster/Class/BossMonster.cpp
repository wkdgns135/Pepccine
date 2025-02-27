#include "Monster/Class/BossMonster.h"
#include "Monster/Component/BerserkModeComponent.h"
#include "Monster/AI/Controller/BossMonsterAIC.h"

ABossMonster::ABossMonster()
{
	// ±¤ÆøÈ­ ¸ğµå ÄÄÆ÷³ÍÆ® ÃÊ±âÈ­
	BerserkModeComponent = CreateDefaultSubobject<UBerserkModeComponent>(TEXT("BerserkModeComponent"));
}
