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
            UMonsterAttackComponent* AttackComp = Owner->FindComponentByClass<UMonsterAttackComponent>();
            if (AttackComp)
            {
                UE_LOG(LogTemp, Log, TEXT("Monster Normal Attack!"));
                AttackComp->AttackTrace();
            }

            else
                UE_LOG(LogTemp, Log, TEXT("No Attack Comp!"));
        }
    }
}