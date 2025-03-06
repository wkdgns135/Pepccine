#include "Character/Components/PrograssBarHUDComponent.h"

UPrograssBarHUDComponent::UPrograssBarHUDComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	StaminaWidget = nullptr;
	HealthWidget = nullptr;
}

void UPrograssBarHUDComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPrograssBarHUDComponent::InitPrograssBar() const
{
	if (StaminaWidget) StaminaWidget->AddToViewport();
	if (HealthWidget) HealthWidget->AddToViewport();
}

void UPrograssBarHUDComponent::SetStamina(const float NewStamina, float MaxStamina) const
{
	if (!StaminaWidget) return;
	StaminaWidget->UpdateStaminaVolumne(NewStamina, MaxStamina);
}

void UPrograssBarHUDComponent::SetHealth(const float NewHealth, const float MaxHealth) const
{
	if (!HealthWidget) return;
	HealthWidget->UpdateHealthVolume(NewHealth, MaxHealth);
}

void UPrograssBarHUDComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
