#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IStaminaObserver.generated.h"

UINTERFACE(MinimalAPI)
class UIStaminaObserver : public UInterface
{
	GENERATED_BODY()

};

class PEPCCINE_API IIStaminaObserver
{
	GENERATED_BODY()

public:
	virtual void OnStaminaChanged(float NewStamina, float MaxStamina) = 0;
};
