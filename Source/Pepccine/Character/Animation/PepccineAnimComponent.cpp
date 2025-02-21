#include "Character/Animation/PepccineAnimComponent.h"

UPepccineAnimComponent::UPepccineAnimComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPepccineAnimComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPepccineAnimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

