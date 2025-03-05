#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "Notify_SkillEnd.generated.h"

UCLASS()
class PEPCCINE_API UNotify_SkillEnd : public UAnimNotify_PlayMontageNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
