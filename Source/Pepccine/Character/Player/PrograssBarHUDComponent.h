#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Widget/StaminaWidget.h"
#include "Character/Widget/HealthWidget.h"
#include "PrograssBarHUDComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEPCCINE_API UPrograssBarHUDComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPrograssBarHUDComponent();

	UPROPERTY(EditAnywhere, Category = "UI")
	UStaminaWidget* StaminaWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	UHealthWidget* HealthWidget;

	void SetStamina(const float NewStamina, const float MaxStamina) const;
	void SetHealth(const float NewHealth, const float MaxHealth) const;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

};
