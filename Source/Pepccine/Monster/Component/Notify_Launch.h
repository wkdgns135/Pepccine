#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "Notify_Launch.generated.h"

UCLASS()
class PEPCCINE_API UNotify_Launch : public UAnimNotify_PlayMontageNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
