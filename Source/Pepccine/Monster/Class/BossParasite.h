#pragma once

#include "CoreMinimal.h"
#include "Monster/Class/BossMonster.h"
#include "BossParasite.generated.h"

class USpawnSkillComponent;

UCLASS()
class PEPCCINE_API ABossParasite : public ABossMonster
{
	GENERATED_BODY()
public:
	ABossParasite();

protected:
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill|Spwan")
	USpawnSkillComponent* SpawnSkillComponent;*/
};
