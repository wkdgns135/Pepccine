#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PepccineAnimComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEPCCINE_API UPepccineAnimComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPepccineAnimComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
