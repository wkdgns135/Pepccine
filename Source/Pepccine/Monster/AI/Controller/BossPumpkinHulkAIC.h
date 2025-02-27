#pragma once

#include "CoreMinimal.h"
#include "Monster/AI/Controller/BossMonsterAIC.h"
#include "BossPumpkinHulkAIC.generated.h"

UCLASS()
class PEPCCINE_API ABossPumpkinHulkAIC : public ABossMonsterAIC
{
	GENERATED_BODY()

public:
	ABossPumpkinHulkAIC();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

	void SetIsInJumpAttackRange(bool InRange);

};
