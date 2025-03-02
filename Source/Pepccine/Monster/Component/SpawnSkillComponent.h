#pragma once

#include "CoreMinimal.h"
#include "Monster/Component/BaseSkillComponent.h"
#include "SpawnSkillComponent.generated.h"

UCLASS(Blueprintable)
class PEPCCINE_API USpawnSkillComponent : public UBaseSkillComponent
{
	GENERATED_BODY()
	
public:
	virtual void ActivateSkill() override;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Skill|Spawn")
	void FadeInMonster(AActor* SpawnedMonster);

	void SpawnMonster();
private:
	UPROPERTY(EditAnywhere, Category = "Skill|Spawn")
	int NumberOfMonsters = 3;  // 몬스터
	UPROPERTY(EditAnywhere, Category = "Skill|Spawn")
	TSubclassOf<class ABaseMonster> MonsterToSpawn;  // 소환할 몬스터 클래스
};
