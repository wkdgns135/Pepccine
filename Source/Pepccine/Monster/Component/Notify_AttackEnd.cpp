#include "Monster/Component/Notify_AttackEnd.h"
#include "Monster/Component/MonsterAttackComponent.h"
#include "Monster/Component/BaseSkillComponent.h"
#include "GameFramework/Actor.h"

void UNotify_AttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (MeshComp)
    {
        AActor* Owner = MeshComp->GetOwner();
        if (Owner)
        {
            // 1. 일반 공격인 경우 MonsterAttackComponent 처리
            UMonsterAttackComponent* AttackComp = Owner->FindComponentByClass<UMonsterAttackComponent>();
            if (AttackComp)
            {
                UE_LOG(LogTemp, Log, TEXT("Monster Normal Attack!"));
                AttackComp->AttackTrace();
            }

            // 2. 스킬 공격인 경우 BaseSkillComponent 처리
            UBaseSkillComponent* SkillComp = Owner->FindComponentByClass<UBaseSkillComponent>();
            if (SkillComp)
            {
                UE_LOG(LogTemp, Log, TEXT("Monster SKill Attack!"));
                // 스킬에 따른 로직 추가 필요
            }
        }
    }
}