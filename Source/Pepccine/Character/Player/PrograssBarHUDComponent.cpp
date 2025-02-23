#include "Character/Player/PrograssBarHUDComponent.h"

UPrograssBarHUDComponent::UPrograssBarHUDComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	StaminaWidget = nullptr;
}

void UPrograssBarHUDComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (StaminaWidget) StaminaWidget->AddToViewport();
}

void UPrograssBarHUDComponent::SetStamina(float NewStamina, float MaxStamina)
{
	if (!StaminaWidget) return;
	StaminaWidget->UpdateStaminaVolumne(NewStamina, MaxStamina);
}

void UPrograssBarHUDComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
