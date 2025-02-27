#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "Notify_AttackEnd.generated.h"

UCLASS()
class PEPCCINE_API UNotify_AttackEnd : public UAnimNotify_PlayMontageNotify
{
    GENERATED_BODY()

public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
