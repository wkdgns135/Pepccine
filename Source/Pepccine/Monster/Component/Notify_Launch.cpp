#include "Monster/Component/Notify_Launch.h"
#include "Monster/Component/JumpAttackComponent.h"

void UNotify_Launch::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (MeshComp)
    {
        AActor* Owner = MeshComp->GetOwner();
        if (Owner)
        {
            UJumpAttackComponent* JumpAttackComp = Owner->FindComponentByClass<UJumpAttackComponent>();
            if (JumpAttackComp)
            {
                UE_LOG(LogTemp, Log, TEXT("Monster Launch!"));
                JumpAttackComp->LaunchMonster();
            }
        }
    }
}
