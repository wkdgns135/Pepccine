#include "Character/Player/CrosshairHUDComponent.h"

UCrosshairHUDComponent::UCrosshairHUDComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCrosshairHUDComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCrosshairHUDComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

