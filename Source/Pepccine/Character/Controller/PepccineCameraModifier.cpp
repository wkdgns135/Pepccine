#include "PepccineCameraModifier.h"
#include "GameFramework/PlayerController.h"



void UPepccineCameraModifier::StartShake(float Strength, float ShakeTime)
{
	TimeElapsed = ShakeTime;
	bIsShaking = true;
	ShakeStrength = 5.0f;
}

void UPepccineCameraModifier::StopShake()
{
	bIsShaking = false;
	ShakeStrength = 0.0f;
}

bool UPepccineCameraModifier::ModifyCamera(float DeltaTime, FMinimalViewInfo& InOutPOV)
{
	InOutPOV.Rotation.Pitch += FMath::RandRange(0.0f, ShakeStrength) * DeltaTime;
	InOutPOV.Rotation.Yaw += FMath::RandRange(-ShakeStrength, ShakeStrength) * DeltaTime;
	InOutPOV.Rotation.Roll += FMath::RandRange(0.0f, 0.0f) * DeltaTime;

	TimeElapsed -= DeltaTime;
	if (TimeElapsed <= 0.0f)
	{
		StopShake();
		return false;
	}

	return true;
}
