#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NormalMonsterAIC.generated.h"


UCLASS()
class PEPCCINE_API ANormalMonsterAIC : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void OnPossess(APawn* InPawn) override; 
	virtual void BeginPlay() override;
};
