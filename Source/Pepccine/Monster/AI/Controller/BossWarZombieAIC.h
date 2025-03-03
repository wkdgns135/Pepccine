#pragma once

#include "CoreMinimal.h"
#include "Monster/AI/Controller/BossMonsterAIC.h"
#include "BossWarZombieAIC.generated.h"

UCLASS()
class PEPCCINE_API ABossWarZombieAIC : public ABossMonsterAIC
{
	GENERATED_BODY()
	
public:
	ABossWarZombieAIC();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
};
