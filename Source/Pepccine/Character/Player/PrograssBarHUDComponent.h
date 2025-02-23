#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaWidget.h"
#include "PrograssBarHUDComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEPCCINE_API UPrograssBarHUDComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPrograssBarHUDComponent();

	UPROPERTY(EditAnywhere, Category = "UI")
	UStaminaWidget* StaminaWidget;

	void SetStamina(float NewStamina, float MaxStamina);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

};
