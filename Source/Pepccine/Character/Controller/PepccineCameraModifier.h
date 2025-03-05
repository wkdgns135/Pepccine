#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraModifier.h"
#include "PepccineCameraModifier.generated.h"

UCLASS()
class PEPCCINE_API UPepccineCameraModifier : public UCameraModifier
{
	GENERATED_BODY()

public:
	void StartShake(float Strength, float ShakeTime);
	void StopShake();

protected:
	virtual bool ModifyCamera(float DeltaTime, FMinimalViewInfo& InOutPOV) override;

private:
	float ShakeStrength;
	float Pitch;
	float Yaw;
	float Roll;
	
	float TimeElapsed = 0.0f;
	bool bIsShaking = false;
};
