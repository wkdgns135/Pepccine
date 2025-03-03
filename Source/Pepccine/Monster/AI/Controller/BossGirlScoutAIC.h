#pragma once

#include "CoreMinimal.h"
#include "Monster/AI/Controller/BossMonsterAIC.h"
#include "BossGirlScoutAIC.generated.h"

UCLASS()
class PEPCCINE_API ABossGirlScoutAIC : public ABossMonsterAIC
{
	GENERATED_BODY()

public: 
	ABossGirlScoutAIC();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

	void SetIsInJumpAttackRange(bool InRange);
};
