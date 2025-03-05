#pragma once

#include "CoreMinimal.h"
#include "Monster/Component/BaseSkillComponent.h"
#include "SpawnSkillComponent.generated.h"

UCLASS(Blueprintable)
class PEPCCINE_API USpawnSkillComponent : public UBaseSkillComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Skill|Spawn")
	void SetNumberOfMonsters(int number) { NumberOfMonsters = number; }

	virtual void ActivateSkill() override;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Skill|Spawn")
	void FadeInMonsters(const TArray<ABaseMonster*>& SpawnedMonsters);

	void SpawnMonster();
private:
	UPROPERTY(EditAnywhere, Category = "Skill|Spawn")
	int NumberOfMonsters = 3;  // 몬스터
	UPROPERTY(EditAnywhere, Category = "Skill|Spawn")
	TSubclassOf<class ABaseMonster> MonsterToSpawn;  // 소환할 몬스터 클래스
    TArray<ABaseMonster*> SpawnedMonsterList;  // 클래스 멤버 변수
};
