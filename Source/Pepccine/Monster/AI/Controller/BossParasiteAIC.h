#pragma once

#include "CoreMinimal.h"
#include "Monster/AI/Controller/BossMonsterAIC.h"
#include "BossParasiteAIC.generated.h"

UCLASS()
class PEPCCINE_API ABossParasiteAIC : public ABossMonsterAIC
{
	GENERATED_BODY()
	
public:
	ABossParasiteAIC();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

};
