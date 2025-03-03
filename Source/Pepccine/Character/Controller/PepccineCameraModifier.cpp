#include "PepccineCameraModifier.h"
#include "GameFramework/PlayerController.h"

void UPepccineCameraModifier::StartShake(float Amplitude, float Frequency, float Duration)
{
    ShakeAmplitude = Amplitude;
    ShakeFrequency = Frequency;
    ShakeDuration = Duration;
    TimeElapsed = 0.0f;
    bIsShaking = true; 
}

void UPepccineCameraModifier::StopShake()
{
    bIsShaking = false; 
}

bool UPepccineCameraModifier::ModifyCamera(float DeltaTime, FMinimalViewInfo& InOutPOV)
{
    if (bIsShaking && TimeElapsed < ShakeDuration)
    {
        float ShakeOffset = FMath::Sin(TimeElapsed * ShakeFrequency) * ShakeAmplitude;

        InOutPOV.Rotation.Yaw += ShakeOffset;
        InOutPOV.Rotation.Pitch += ShakeOffset * 0.5f;

        TimeElapsed += DeltaTime;
        return true;
    }

    if (TimeElapsed >= ShakeDuration)
    {
        StopShake();
    }

    return false;
}