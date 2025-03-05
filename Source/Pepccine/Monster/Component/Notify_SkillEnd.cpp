#include "Monster/Component/Notify_SkillEnd.h"
#include "Monster/Component/BaseSkillComponent.h"

void UNotify_SkillEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (MeshComp)
    {
        AActor* Owner = MeshComp->GetOwner();
        if (Owner)
        {
            UBaseSkillComponent* SkillComp = Owner->FindComponentByClass<UBaseSkillComponent>();
            if (SkillComp)
            {
                UE_LOG(LogTemp, Log, TEXT("Monster SKill Attack!"));
                SkillComp->SkillTrace();
            }
        }
    }
}