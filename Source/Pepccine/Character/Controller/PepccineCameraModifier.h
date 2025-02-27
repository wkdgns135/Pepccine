#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraModifier.h"
#include "PepccineCameraModifier.generated.h"

UCLASS()
class PEPCCINE_API UPepccineCameraModifier : public UCameraModifier
{
	GENERATED_BODY()

public:
	void StartShake(float Amplitude, float Frequency, float Duration);
	void StopShake();

protected:
	virtual bool ModifyCamera(float DeltaTime, FMinimalViewInfo& InOutPOV) override;

private:
	float ShakeAmplitude = 0.0f;  // 흔들림 강도
	float ShakeFrequency = 0.0f;  // 흔들림 속도
	float ShakeDuration = 0.0f;   // 흔들림 지속 시간
	float TimeElapsed = 0.0f;     // 흔들린 시간 추적
	bool bIsShaking = false;      // 현재 흔들리고 있는지 여부
};
